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


#ifndef MAR_ENGINE_PLATFORMS_TEXTURE_OPENGL_H
#define MAR_ENGINE_PLATFORMS_TEXTURE_OPENGL_H

#include "../../mar.h"
#include "../PlatformLogs.h"


namespace mar {
	namespace platforms {


		/*!
			TextureOpenGL is used to bind textures to renderer objects.
			This class expects, that every renderer shape is containing unique ID, which
			will be prescribed to texture ID.
			This is overloaded class of base one - Texture, which implements
			solutions to specified problems using OpenGL.
		*/
		class TextureOpenGL {
			static std::unordered_map<std::string, uint32_t> s_2d;
			static std::unordered_map<std::string, uint32_t> s_cubemaps;

		public:

			// Delete all existing textures
			void shutdown();

			/*
			Load 2D texture and prescribe it to available index. If texture is loaded
			it pushes its index again.

				\param path - path to texture, which will be loaded
				\return id - id for new loaded texture
			*/
			uint32_t loadTexture(std::string path);

			/*
			Method loads Cube Map and prescribe it to available index.
			Method must be overloaded!

				\param faces - paths to textures, which will be loaded
			*/
			uint32_t loadCubemap(std::string path);

			/*
			Bind texture with texID to selected shape with shapeId.

				\param texture_type - type ex. GL_TEXTURE_2D / GL_TEXTURE_CUBE_MAP
				\param unit - id of shape
				\param texID - id of texture
			*/
			void bind(uint32_t texture_type, uint32_t unit, uint32_t texID) const;

			/*
			Bind tex_id of GL_TEXTURE_2D to selected unit

				\param unit - sampler
				\param tex_id - id of texture2D
			*/
			void bind2D(uint32_t unit, uint32_t tex_id) const;

			/*
			Bind cube_id of GL_TEXTURE_CUBE_MAP to selected unit

				\param unit - sampler
				\param cube_id - id of Texture Cubemap
			*/
			void bindCube(uint32_t unit, uint32_t cube_id) const;

			/*
			Unbinds everething, given vector informs, if texture2d or cubemap should be unbinded from unit

				\param texture_types - 1.0f for Texture2D, 2.0f for Cubemap
			*/
			void unbind(const std::vector<float>& texture_types) const;

			/*
			Checks, if instance has loaded given key (which is path to texture)

				\param key - selected texture, which will be checked
				\returns bool - true, if it is loaded, false otherwise
			*/
			static bool hasTexture(std::string key);

			/*
			Returns if id of given texture2D (path to it)
				
				\param key - selected texture2D
				\returns uint32_t - id of selected texture2D
			*/
			static uint32_t getTexture(std::string key) { return s_2d.at(key); }

			/*
			Returns if id of given Cubemap (path to it)

				\param key - selected Cubemap
				\returns uint32_t - id of selected Cubemap
			*/
			static uint32_t getCubemap(std::string key) { return s_cubemaps.at(key); }

		private:
			/*
			Method generates new id for texture.

				\param path - path to texture, which will be loaded
				\return id - id for new loaded texture
			*/
			uint32_t genNewTexture(const char* path);

			/*
			Method generates new id for texture. Must be overloaded!

				\param faces - paths to textures, which will be loaded
				\return id - id for new loaded texture
			*/
			uint32_t genNewCubemap(const char* path);
		};


} }

#endif // !MAR_ENGINE_TEXTURE_OPENGL_H