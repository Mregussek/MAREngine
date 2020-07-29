/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef TEXTURE_OPENGL_H
#define TEXTURE_OPENGL_H

#include "Texture.h"
#include "../../../Debug/Log.h"


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
			static uint32_t s_textureUnit;
			std::vector<uint32_t> m_id;
			std::vector<std::string> m_paths;
			std::unordered_map<std::string, uint32_t> m_path_id;

		public:

			// Delete all existing textures
			void shutdown();

			/*
			Method generates new id for texture.

				\param path - path to texture, which will be loaded
				\return id - id for new loaded texture
			*/
			uint32_t genNewTexture(const char* path);

			/*
			Load 2D texture and prescribe it to available index. If texture is loaded
			it pushes its index again.

				\param path - path to texture, which will be loaded
				\return id - id for new loaded texture
			*/
			float loadTexture(const char* path);

			/*
			Method generates new id for cubemap. Must be overloaded!

				\param faces - paths to textures, which will be loaded
				\return id - id for new loaded cubemap
			*/
			uint32_t genNewCubemap(const char* path);

			/*
			Method loads Cube Map and prescribe it to available index.
			Method must be overloaded!

				\param faces - paths to textures, which will be loaded
			*/
			float loadCubemap(const char* path);

			/*
			Bind texture with texID to selected shape with shapeId.

				\param shapeId - id of shape
				\param texID - id of texture
			*/
			void bind(const int& shapeId, const uint32_t& texID) const;

			// Set default texture for each shape
			void unbind() const;

			/*
			Adds id prescribed to shape

				\param id - new id
			*/
			void addID(const uint32_t id);

			/*
			Removes id prescribed to shape with specified index

				\param index - index of shape
			*/
			void removeID(const uint32_t& index);

			/*
			Get id of texture bounded to shape on specific index

				\param index - index of shape
				\return m_id[index] - id of texture prescribed to shape
			*/
			const uint32_t& getID(const uint32_t& index) const { return m_id[index]; }
 
			/*
			Get path to texture associated to shape with specified index.
			Method must be overloaded!

				\param index - index of path
				\return m_path[index] - path of texture prescribed to shape
			*/
			const std::string& getPath(const uint32_t& index) const { return m_paths[index]; }

			/*
			Method resets static variable - texture unit.
			*/
			void resetTextureUnit() { s_textureUnit = 1; }
		};


} }

#endif // !TEXTURE_OPENGL_H