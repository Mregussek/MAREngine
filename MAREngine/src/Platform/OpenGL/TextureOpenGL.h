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


#ifndef MAR_ENGINE_PLATFORMS_TEXTURE_OPENGL_H
#define MAR_ENGINE_PLATFORMS_TEXTURE_OPENGL_H


#include "../../mar.h"


namespace marengine {


	class TextureOpenGL {
	public:

		static TextureOpenGL* Instance() { return s_instance; }

		void shutdown();

		uint32_t loadTexture(std::string path);
		uint32_t loadCubemap(std::string path);

		void bind2D(uint32_t unit, uint32_t tex_id) const;
		void bindCube(uint32_t unit, uint32_t cube_id) const;
		void unbind(const std::vector<float>& texture_types) const;

		static bool hasTexture(std::string key);
		static bool hasCubemap(std::string key);

		static uint32_t getTexture(std::string key);
		static uint32_t getCubemap(std::string key);

	private:

		uint32_t genNewTexture(const char* path);
		uint32_t genNewCubemap(const std::string& path);


		static std::unordered_map<std::string, uint32_t> s_2d;
		static std::unordered_map<std::string, uint32_t> s_cubemaps;
		static TextureOpenGL* s_instance;

	};


}


#endif // !MAR_ENGINE_TEXTURE_OPENGL_H