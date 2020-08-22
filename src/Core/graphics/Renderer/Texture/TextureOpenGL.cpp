/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "TextureOpenGL.h"


namespace mar {
	namespace graphics {


		void TextureOpenGL::shutdown() {
			for (auto& tex : m_2d) {
				MAR_CORE_GL_FUNC( glDeleteTextures(1, &tex.second) );
			}
			for (auto& tex : m_cubemaps) {
				MAR_CORE_GL_FUNC( glDeleteTextures(1, &tex.second) );
			}

			GRAPHICS_INFO("TEXTURE_OPENGL: deleting all textures");
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
					GRAPHICS_ERROR(
						"TEXTURE_OPENGL: Format from texture is not supported!"
						"bitPerPixel: {0:d}",  bitPerPixel
					);

					return 0;
				}

				MAR_CORE_GL_FUNC ( glCreateTextures(GL_TEXTURE_2D, 1, &id) );
				MAR_CORE_GL_FUNC ( glTextureStorage2D(id, 1, internalFormat, width, height) );
				MAR_CORE_GL_FUNC ( glTextureSubImage2D(id, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, localBuffer) );

				MAR_CORE_GL_FUNC ( glGenerateMipmap(GL_TEXTURE_2D) );

				MAR_CORE_GL_FUNC ( glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE) );
				MAR_CORE_GL_FUNC ( glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE) );
			
				MAR_CORE_GL_FUNC ( glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR) );
				MAR_CORE_GL_FUNC ( glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR) );

				stbi_image_free(localBuffer);

				GRAPHICS_TRACE("TEXTURE_OPENGL: {} 2D texture loaded successfully - assigned to: {}!", path, id);

				return id;
			}

			GRAPHICS_ERROR("TEXTURE_OPENGL: Failed to load {}", path);

			return 0;
		}

		uint32_t TextureOpenGL::loadTexture(const std::string& path) {
			auto search = m_2d.find(path);

			if (search != m_2d.end()) { 
				GRAPHICS_TRACE("TEXTURE_OPENGL: Assigning loaded 2D texture {} - {}!", search->first, search->second);
				return search->second;
			}

			uint32_t new_id = genNewTexture(path.c_str());
			m_2d.insert({ path, new_id });
			return new_id;
		}

		uint32_t TextureOpenGL::genNewCubemap(const char* path) {
			std::vector<std::string> faces(6);
			faces[0] = std::string(path) + "/right.jpg";
			faces[1] = std::string(path) + "/left.jpg";
			faces[2] = std::string(path) + "/top.jpg";
			faces[3] = std::string(path) + "/bottom.jpg";
			faces[4] = std::string(path) + "/front.jpg";
			faces[5] = std::string(path) + "/back.jpg";

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
					GRAPHICS_ERROR("TEXTURE_OPENGL: Cubemap texture failed to load! Path: {}, Face: {}", path, faces[i]);

					stbi_image_free(localBuffer);
				}
			}

			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

			GRAPHICS_TRACE("TEXTURE_OPENGL: {} cubemap loaded successfully and assigned to: {}!", path, id);

			return id;
		}

		uint32_t TextureOpenGL::loadCubemap(const std::string& path) {
			auto search = m_cubemaps.find(path);

			if (search != m_cubemaps.end()) {
				GRAPHICS_TRACE("TEXTURE_OPENGL: Assigning loaded cubemap {} - {}!", search->first, search->second);
				return search->second;
			}

			uint32_t new_id = genNewCubemap(path.c_str());
			m_cubemaps.insert({ path, new_id });

			return new_id;
		}

		void TextureOpenGL::bind(uint32_t texture_type, uint32_t unit, uint32_t texID) const {
			if (texID == 0) return;

			MAR_CORE_GL_FUNC( glActiveTexture(GL_TEXTURE0 + unit) );
			MAR_CORE_GL_FUNC( glBindTexture(texture_type, texID) );

			GRAPHICS_TRACE("TEXTURE_OPENGL: Binding - glActiveTexture(GL_TEXTURE0 + {}) - glBindTexture({}, {})", unit, (texture_type == GL_TEXTURE_2D ? "Texture2D" : "Cubemap"), texID);
		}

		void TextureOpenGL::unbind() const {
			MAR_CORE_GL_FUNC( glBindTexture(GL_TEXTURE_2D, 0) );

			GRAPHICS_TRACE("TEXTURE_OPENGL: Unbinding texture");
		}


} }