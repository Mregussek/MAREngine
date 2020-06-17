/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "TextureOpenGL.h"

namespace mar {
	namespace graphics {


		void TextureOpenGL::shutdown() {
			for (auto const& id : m_id)
				glDeleteTextures(1, &id);
		}

		unsigned int TextureOpenGL::genNewTexture(const std::string& path) {
			unsigned int id;
			int width, height;
			int bitPerPixel;
			unsigned char* localBuffer;

			glGenTextures(1, &id);
			glBindTexture(GL_TEXTURE_2D, id);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_set_flip_vertically_on_load(true);
			localBuffer = stbi_load(path.c_str(), &width, &height, &bitPerPixel, 0);

			if (localBuffer) {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, localBuffer);
				glGenerateMipmap(GL_TEXTURE_2D);
				stbi_image_free(localBuffer);

				MAR_CORE_TRACE("Texture loaded successfully!");

				return id;
			}

			MAR_CORE_ERROR("Failed to load texture");

			return 0;
		}

		void TextureOpenGL::loadTexture(const std::string& path) {
			auto search = m_path_id.find(path);

			if (search != m_path_id.end()) { 
				m_id.push_back(search->second);
				m_paths.push_back(search->first);

				MAR_CORE_TRACE("Assigning loaded texture!");

				return;
			}

			unsigned int new_id = genNewTexture(path);
			m_id.push_back(new_id);
			m_paths.push_back(path);
			m_path_id.insert({ path, new_id });
		}

		unsigned int TextureOpenGL::genNewCubemap(const std::vector<std::string>& faces) {
			unsigned int id;
			int width, height;
			int bitPerPixel;

			glGenTextures(1, &id);
			glBindTexture(GL_TEXTURE_CUBE_MAP, id);
			stbi_set_flip_vertically_on_load(false);

			for (unsigned int i = 0; i < faces.size(); i++) {

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

			MAR_CORE_TRACE("Cube Map loaded successfully!");

			return id;
		}

		void TextureOpenGL::loadCubemap(const std::vector<std::string>& faces) {
			std::string helper = faces[0] + faces[1] + faces[2];
			auto search = m_path_id.find(helper);

			if (search != m_path_id.end()) {
				m_id.push_back(search->second);
				m_paths.push_back(search->first);

				MAR_CORE_TRACE("Assigning loaded cubemap!");

				return;
			}

			unsigned int new_id = genNewCubemap(faces);
			m_id.push_back(new_id);
			m_paths.push_back(helper);
			m_path_id.insert({ helper, new_id });
		}

		void TextureOpenGL::bind(const int& shapeId, const unsigned int& texID) const {
			glBindTextureUnit((unsigned int)shapeId, texID);
		}

		void TextureOpenGL::unbind() const {
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		void TextureOpenGL::addID(const unsigned int id) {
			m_id.push_back(id);
			m_paths.push_back("empty");
		}

		void TextureOpenGL::removeID(const unsigned int& index) {
			unsigned int id_count = std::count(m_id.begin(), m_id.end(), m_id[index]);

			if (id_count == 1 && m_id[index] != 0) {
				std::string path_to_find = m_paths[index];
				auto it = m_path_id.find(path_to_find);

				if (it != m_path_id.end()) m_path_id.erase(it);
				else MAR_CORE_ERROR("Could not delete last occurence of texture!");

				glDeleteTextures(1, &m_id[index]); 
			}

			for (unsigned int i = index; i < m_id.size() - 1; i++) { 
				m_id[i] = m_id[i + 1]; 
				m_paths[i] = m_paths[i + 1];
			}
				
			m_id.pop_back();
			m_paths.pop_back();
		}


} }