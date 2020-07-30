/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "TextureOpenGL.h"


namespace mar {
	namespace graphics {


		uint32_t TextureOpenGL::s_textureUnit = 1;

		void TextureOpenGL::shutdown() {
			for (auto const& id : m_id) {
				MAR_CORE_GL_FUNC( glDeleteTextures(1, &id) );
			}
		}

		uint32_t TextureOpenGL::genNewTexture(const char* path) {
			int width, height;
			int bitPerPixel;
			unsigned char* localBuffer;

			stbi_set_flip_vertically_on_load(true);
			localBuffer = stbi_load(path, &width, &height, &bitPerPixel, 0);

			if (localBuffer) {
				uint32_t id;
				GLenum internalFormat = 0;
				GLenum dataFormat = 0;

				if (bitPerPixel == 4) {
					internalFormat = GL_RGBA8;
					dataFormat = GL_RGBA;
				}
				else if (bitPerPixel == 3) {
					internalFormat = GL_RGB8;
					dataFormat = GL_RGB;
				}
				else {
					GRAPHICS_ERROR("TEXTURE_OPENGL: Format from texture is not supported!");

					return 0;
				}

				MAR_CORE_GL_FUNC ( glCreateTextures(GL_TEXTURE_2D, 1, &id) );
				MAR_CORE_GL_FUNC ( glTextureStorage2D(id, 1, internalFormat, width, height) );
				MAR_CORE_GL_FUNC ( glTextureSubImage2D(id, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, localBuffer) );

				MAR_CORE_GL_FUNC ( glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE) );
				MAR_CORE_GL_FUNC ( glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE) );
			
				MAR_CORE_GL_FUNC ( glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR) );
				MAR_CORE_GL_FUNC ( glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR) );

				stbi_image_free(localBuffer);

				GRAPHICS_TRACE("TEXTURE_OPENGL: Texture loaded successfully!");

				return id;
			}

			GRAPHICS_ERROR("TEXTURE_OPENGL: Failed to load texture");

			return 0;
		}

		float TextureOpenGL::loadTexture(const char* path) {
			auto search = m_path_id.find(path);

			if (search != m_path_id.end()) { 
				m_id.push_back(search->second);
				m_paths.push_back(search->first);

				GRAPHICS_TRACE("TEXTURE_OPENGL: Assigning loaded texture!");

				return (float) search->second;
			}

			uint32_t new_id = genNewTexture(path);
			m_id.push_back(new_id);
			m_paths.push_back(path);
			m_path_id.insert({ path, new_id });
			return (float) new_id;
		}

		uint32_t TextureOpenGL::genNewCubemap(const char* path) {
			std::vector<std::string> faces(6);
			int check = 0;

			for (const auto& entry : std::filesystem::directory_iterator(path)) {
				check++;
			}

			if (check == 6) {
				faces[0] = std::string(path) + "/right.jpg";
				faces[1] = std::string(path) + "/left.jpg";
				faces[2] = std::string(path) + "/top.jpg";
				faces[3] = std::string(path) + "/bottom.jpg";
				faces[4] = std::string(path) + "/front.jpg";
				faces[5] = std::string(path) + "/back.jpg";
			}
			else {
				GRAPHICS_ERROR("TEXTURE_OPENGL: Cannot load 6 files for cubemap!");
				return 0;
			}

			uint32_t id;
			int width, height;
			int bitPerPixel;

			glGenTextures(1, &id);
			glBindTexture(GL_TEXTURE_CUBE_MAP, id);
			stbi_set_flip_vertically_on_load(false);

			for (uint32_t i = 0; i < faces.size(); i++) {

				unsigned char* localBuffer = stbi_load(faces[i].c_str(), &width, &height, &bitPerPixel, 0);

				if (localBuffer) {
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, localBuffer);
					stbi_image_free(localBuffer);
				}
				else {
					MAR_CORE_ERROR("Cube map texture failed to load!");

					stbi_image_free(localBuffer);
				}
			}

			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

			GRAPHICS_TRACE("TEXTURE_OPENGL: Cube Map loaded successfully!");

			return id;
		}

		float TextureOpenGL::loadCubemap(const char* path) {
			std::string helper = std::string(path);
			auto search = m_path_id.find(helper);

			if (search != m_path_id.end()) {
				m_id.push_back(search->second);
				m_paths.push_back(search->first);

				GRAPHICS_TRACE("TEXTURE_OPENGL: Assigning loaded cubemap!");

				return (float) search->second;
			}

			uint32_t new_id = genNewCubemap(path);
			m_id.push_back(new_id);
			m_paths.push_back(helper);
			m_path_id.insert({ helper, new_id });

			return (float) new_id;
		}

		void TextureOpenGL::bind(const int& texture_type, const uint32_t& texID) const {
			if (texID == 0) return;

			MAR_CORE_GL_FUNC( glActiveTexture(GL_TEXTURE0 + s_textureUnit) );
			MAR_CORE_GL_FUNC( glBindTexture(texture_type, texID) );

			s_textureUnit++;
		}

		void TextureOpenGL::unbind() const {
			MAR_CORE_GL_FUNC( glBindTexture(GL_TEXTURE_2D, 0) );
		}

		void TextureOpenGL::addID(const uint32_t id) {
			m_id.push_back(id);
			m_paths.push_back("empty");
		}

		void TextureOpenGL::removeID(const uint32_t& index) {
			uint32_t id_count = std::count(m_id.begin(), m_id.end(), m_id[index]);

			if (id_count == 1 && m_id[index] != 0) {
				std::string path_to_find = m_paths[index];
				auto it = m_path_id.find(path_to_find);

				if (it != m_path_id.end()) m_path_id.erase(it);
				else GRAPHICS_ERROR("TEXTURE_OPENGL: Could not delete last occurence of texture!");

				MAR_CORE_GL_FUNC( glDeleteTextures(1, &m_id[index]) );
			}

			for (uint32_t i = index; i < m_id.size() - 1; i++) { 
				m_id[i] = m_id[i + 1]; 
				m_paths[i] = m_paths[i + 1];
			}
				
			m_id.pop_back();
			m_paths.pop_back();

			return;
		}


} }