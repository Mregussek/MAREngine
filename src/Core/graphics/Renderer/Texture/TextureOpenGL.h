/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_TEXTURE_OPENGL_H
#define MAR_ENGINE_TEXTURE_OPENGL_H

#include "Texture.h"
#include "../../GraphicsLogs.h"


namespace mar {
	namespace graphics {


		/*!
			TextureOpenGL is used to bind textures to renderer objects.
			This class expects, that every renderer shape is containing unique ID, which
			will be prescribed to texture ID.
			This is overloaded class of base one - Texture, which implements
			solutions to specified problems using OpenGL.
		*/
		class TextureOpenGL {
			std::unordered_map<std::string, uint32_t> m_2d;
			std::unordered_map<std::string, uint32_t> m_cubemaps;

		public:

			// Delete all existing textures
			void shutdown();

			/*
			Load 2D texture and prescribe it to available index. If texture is loaded
			it pushes its index again.

				\param path - path to texture, which will be loaded
				\return id - id for new loaded texture
			*/
			float loadTexture(const std::string& path);

			/*
			Method loads Cube Map and prescribe it to available index.
			Method must be overloaded!

				\param faces - paths to textures, which will be loaded
			*/
			float loadCubemap(const std::string& path);

			/*
			Bind texture with texID to selected shape with shapeId.

				\param shapeId - id of shape
				\param texID - id of texture
			*/
			void bind(int32_t texture_type, uint32_t unit, uint32_t texID) const;

			// Unbind texture
			void unbind() const;

			const uint32_t& getTexture(const std::string& key) const { return m_2d.at(key); }

			const uint32_t& getCubemap(const std::string& key) const { return m_cubemaps.at(key); }

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