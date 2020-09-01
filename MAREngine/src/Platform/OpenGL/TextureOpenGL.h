/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

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

			void bind2D(uint32_t unit, uint32_t tex_id) const;

			void bindCube(uint32_t unit, uint32_t cube_id) const;

			// Unbind texture
			void unbind(const std::vector<float>& texture_types) const;

			static bool hasTexture(std::string key);

			static uint32_t getTexture(std::string key) { return s_2d.at(key); }

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