/***********************************************************************
* @internal @copyright
*
*  				MAREngine - open source 3D game engine
*
* Copyright (C) 2020-present Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
* All rights reserved.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
************************************************************************/


#include "TextureOpenGL.h"
#include "../../ProjectManager.h"


namespace marengine {


	std::unordered_map<std::string, uint32_t> TextureOpenGL::s_2d;
	std::unordered_map<std::string, uint32_t> TextureOpenGL::s_cubemaps;
	TextureOpenGL* TextureOpenGL::s_instance{ nullptr };


	void TextureOpenGL::shutdown() {
		for (auto& tex : s_2d) {
			PLATFORM_GL_FUNC( glDeleteTextures(1, &tex.second) );
		}
		for (auto& tex : s_cubemaps) {
			PLATFORM_GL_FUNC( glDeleteTextures(1, &tex.second) );
		}

		s_2d.clear();
		s_cubemaps.clear();
	}

	uint32_t TextureOpenGL::genNewTexture(const char* path) {
		int32_t width, height;
		int32_t bitPerPixel;
		unsigned char* localBuffer;

		stbi_set_flip_vertically_on_load(true);
		localBuffer = stbi_load(path, &width, &height, &bitPerPixel, 0);

		if (localBuffer) {
			uint32_t id{ 0 };
			GLenum internalFormat{ 0 };
			GLenum dataFormat{ 0 };

			if (bitPerPixel == 4) {
				internalFormat = GL_RGBA8;
				dataFormat = GL_RGBA;
			}
			else if (bitPerPixel == 3) {
				internalFormat = GL_RGB8;
				dataFormat = GL_RGB;
			}
			else {
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

			return id;
		}

		return 0;
	}

	uint32_t TextureOpenGL::loadTexture(std::string path) {
		const auto search = s_2d.find(std::move(path));

		if (search != s_2d.cend()) {
			return search->second;
		}

		const std::string assetsTexturePath{ ProjectManager::Instance->getAssetsPath() + path };
		const uint32_t id = genNewTexture(assetsTexturePath.c_str());
		s_2d.insert({ path, id });
		return id;
	}

	uint32_t TextureOpenGL::genNewCubemap(const std::string& path) {
		const std::array<const std::string, 6> faces{
			 path + "/right.jpg",
			 path + "/left.jpg",
			 path + "/top.jpg",
			 path + "/bottom.jpg",
			 path + "/front.jpg",
			 path + "/back.jpg"
		};

		uint32_t id;
		int32_t width, height;
		int32_t bitPerPixel;

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
				stbi_image_free(localBuffer);
			}
		}

		PLATFORM_GL_FUNC ( glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR) );
		PLATFORM_GL_FUNC ( glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR) );

		PLATFORM_GL_FUNC ( glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE) );
		PLATFORM_GL_FUNC ( glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE) );
		PLATFORM_GL_FUNC ( glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE) );

		return id;
	}

	uint32_t TextureOpenGL::loadCubemap(std::string path) {
		const auto search = s_cubemaps.find(std::move(path));

		if (search != s_cubemaps.cend()) {
			return search->second;
		}

		const std::string assetsTexturePath{ ProjectManager::Instance->getAssetsPath() + path };
		const uint32_t id = genNewCubemap(assetsTexturePath);
		s_cubemaps.insert({ path, id });

		return id;
	}

	bool TextureOpenGL::hasTexture(std::string key) {
		auto search = s_2d.find(std::move(key));
		if (search != s_2d.end()) 
			return true;

		return false;
	}

	bool TextureOpenGL::hasCubemap(std::string key) {
		auto search = s_cubemaps.find(std::move(key));
		if (search != s_cubemaps.end())
			return true;

		return false;
	}

	uint32_t TextureOpenGL::getTexture(std::string key) { 
		return s_2d.at(std::move(key)); 
	}

	uint32_t TextureOpenGL::getCubemap(std::string key) { 
		return s_cubemaps.at(std::move(key));
	}

	void TextureOpenGL::bind2D(uint32_t unit, uint32_t tex_id) const {
		PLATFORM_GL_FUNC( glActiveTexture(GL_TEXTURE0 + unit) );
		PLATFORM_GL_FUNC( glBindTexture(GL_TEXTURE_2D, tex_id) );
	}

	void TextureOpenGL::bindCube(uint32_t unit, uint32_t cube_id) const {
		PLATFORM_GL_FUNC( glActiveTexture(GL_TEXTURE0 + unit) );
		PLATFORM_GL_FUNC( glBindTexture(GL_TEXTURE_CUBE_MAP , cube_id));
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
	}


}
