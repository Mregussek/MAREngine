/**
 *           MAREngine - open source 3D game engine
 * Copyright (C) 2020-present Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 * All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
**/


#include "TextureOpenGL.h"


namespace mar {
	namespace platforms {


		std::unordered_map<std::string, uint32_t> TextureOpenGL::s_2d;
		std::unordered_map<std::string, uint32_t> TextureOpenGL::s_cubemaps;


		void TextureOpenGL::shutdown() {
			for (auto& tex : s_2d) {
				PLATFORM_GL_FUNC( glDeleteTextures(1, &tex.second) );
			}
			for (auto& tex : s_cubemaps) {
				PLATFORM_GL_FUNC( glDeleteTextures(1, &tex.second) );
			}

			s_2d.clear();
			s_cubemaps.clear();

			PLATFORM_INFO("TEXTURE_OPENGL: deleting all textures");
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
					PLATFORM_ERROR(
						"TEXTURE_OPENGL: Format from texture is not supported!"
						"bitPerPixel: {0:d}",  bitPerPixel
					);

					return 0;
				}

				PLATFORM_GL_FUNC ( glCreateTextures(GL_TEXTURE_2D, 1, &id) );
				PLATFORM_GL_FUNC ( glTextureStorage2D(id, 1, internalFormat, width, height) );
				PLATFORM_GL_FUNC ( glTextureSubImage2D(id, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, localBuffer) );

				PLATFORM_GL_FUNC ( glGenerateMipmap(GL_TEXTURE_2D) );

				PLATFORM_GL_FUNC ( glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE) );
				PLATFORM_GL_FUNC ( glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE) );
			
				PLATFORM_GL_FUNC ( glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR) );
				PLATFORM_GL_FUNC ( glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR) );

				stbi_image_free(localBuffer);

				PLATFORM_TRACE("TEXTURE_OPENGL: {} 2D texture loaded successfully - assigned to: {}!", path, id);

				return id;
			}

			PLATFORM_ERROR("TEXTURE_OPENGL: Failed to load {}", path);

			return 0;
		}

		uint32_t TextureOpenGL::loadTexture(std::string path) {
			auto search = s_2d.find(path);

			if (search != s_2d.end()) {
				PLATFORM_TRACE("TEXTURE_OPENGL: Assigning loaded 2D texture {} - {}!", search->first, search->second);
				return search->second;
			}

			uint32_t new_id = genNewTexture(path.c_str());
			s_2d.insert({ path, new_id });
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

			PLATFORM_GL_FUNC( glGenTextures(1, &id) );
			PLATFORM_GL_FUNC( glBindTexture(GL_TEXTURE_CUBE_MAP, id) );
			stbi_set_flip_vertically_on_load(false);

			for (uint32_t i = 0; i < faces.size(); i++) {

				unsigned char* localBuffer = stbi_load(faces[i].c_str(), &width, &height, &bitPerPixel, 0);

				if (localBuffer) {
					PLATFORM_GL_FUNC( glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, localBuffer) );

					stbi_image_free(localBuffer);
				}
				else {
					PLATFORM_ERROR("TEXTURE_OPENGL: Cubemap texture failed to load! Path: {}, Face: {}", path, faces[i]);

					stbi_image_free(localBuffer);
				}
			}

			PLATFORM_GL_FUNC ( glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR) );
			PLATFORM_GL_FUNC ( glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR) );

			PLATFORM_GL_FUNC ( glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE) );
			PLATFORM_GL_FUNC ( glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE) );
			PLATFORM_GL_FUNC ( glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE) );

			PLATFORM_TRACE("TEXTURE_OPENGL: {} cubemap loaded successfully and assigned to: {}!", path, id);

			return id;
		}

		uint32_t TextureOpenGL::loadCubemap(std::string path) {
			auto search = s_cubemaps.find(path);

			if (search != s_cubemaps.end()) {
				PLATFORM_TRACE("TEXTURE_OPENGL: Assigning loaded cubemap {} - {}!", search->first, search->second);
				return search->second;
			}

			uint32_t new_id = genNewCubemap(path.c_str());
			s_cubemaps.insert({ path, new_id });

			return new_id;
		}

		bool TextureOpenGL::hasTexture(std::string key) {
			auto search = s_2d.find(key);
			if (search != s_2d.end()) 
				return true;

			return false;
		}

		void TextureOpenGL::bind(uint32_t texture_type, uint32_t unit, uint32_t texID) const {
			PLATFORM_GL_FUNC( glActiveTexture(GL_TEXTURE0 + unit) );
			PLATFORM_GL_FUNC( glBindTexture(texture_type, texID) );

			PLATFORM_TRACE("TEXTURE_OPENGL: Binding - glActiveTexture(GL_TEXTURE0 + {}) - glBindTexture({}, {})", unit, (texture_type == GL_TEXTURE_2D ? "Texture2D" : "Cubemap"), texID);
		}

		void TextureOpenGL::bind2D(uint32_t unit, uint32_t tex_id) const {
			PLATFORM_GL_FUNC( glActiveTexture(GL_TEXTURE0 + unit) );
			PLATFORM_GL_FUNC( glBindTexture(GL_TEXTURE_2D, tex_id) );

			PLATFORM_TRACE("TEXTURE_OPENGL: Binding2D - glActiveTexture(GL_TEXTURE0 + {}) - glBindTexture(GL_TEXTURE_2D, {})", unit, tex_id);
		}

		void TextureOpenGL::bindCube(uint32_t unit, uint32_t cube_id) const {
			PLATFORM_GL_FUNC( glActiveTexture(GL_TEXTURE0 + unit) );
			PLATFORM_GL_FUNC( glBindTexture(GL_TEXTURE_CUBE_MAP , cube_id));

			PLATFORM_TRACE("TEXTURE_OPENGL: BindingCube - glActiveTexture(GL_TEXTURE0 + {}) - glBindTexture(GL_TEXTURE_CUBE_MAP, {})", unit, cube_id);
		}

		void TextureOpenGL::unbind(const std::vector<float>& texture_types) const {
			for (uint32_t i = 0; i < texture_types.size(); i++) {
				PLATFORM_GL_FUNC( glActiveTexture(GL_TEXTURE0 + i) );

				if (texture_types[i] == 1.0f) {
					PLATFORM_GL_FUNC( glBindTexture(GL_TEXTURE_2D, 0) );
				}
				else if (texture_types[i] == 2.0f) {
					PLATFORM_GL_FUNC( glBindTexture(GL_TEXTURE_CUBE_MAP, 0) );
				}
			}

			PLATFORM_TRACE("TEXTURE_OPENGL: Unbinding texture");
		}


} }