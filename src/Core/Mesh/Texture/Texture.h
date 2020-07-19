/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef TEXTURE_H
#define TEXTURE_H

#include "../../../mar.h"
#include "TexturePaths.h"
#include "../../../Debug/Log.h"


namespace mar {
	namespace graphics {


		/*!
			Texture is used to bind textures to renderer objects.
			This class expects, that every renderer shape is containing unique ID, which
			will be prescribed to texture ID.
			This is just base class for other implementations of Texture.
		*/
		template<typename TEX>
		class Texture {
			TEX m_texture;

		public:

			// This method should deleting whole stuff associated to Textures.
			// Method must be overloaded!
			void shutdown() {
				m_texture.shutdown();
			}

			/*
			Method should generate new id for texture. Must be overloaded!

				\param path - path to texture, which will be loaded
				\return id - id for new loaded texture
			*/
			uint32_t genNewTexture(const char* path) { 
				return m_texture.genNewTexture(path);
			}

			/*
			Method should load 2D texture and prescribe it to available index.
			Method must be overloaded!

				\param path - path to texture, which will be loaded
			*/
			float loadTexture(const char* path) { 
				return m_texture.loadTexture(path);
			}

			/*
			Method should generate new id for cubemap. Must be overloaded!

				\param faces - paths to textures, which will be loaded
				\return id - id for new loaded cubemap
			*/
			uint32_t genNewCubemap(const char* path) { 
				return m_texture.genNewCubemap(path);
			}

			/*
			Method should load Cube Map and prescribe it to available index.
			Method must be overloaded!

				\param faces - paths to textures, which will be loaded
			*/
			float loadCubemap(const char* path) { 
				return m_texture.loadCubemap(path); 
			}

			/*
			Method should bind texture (with unique ID) to selected shape (with unique ID)
			Method must be overloaded!

				\param shapeId - id of shape
				\param texID - id of texture
			*/
			void bind(const int& shapeId, const uint32_t& texID) const {
				m_texture.bind(shapeId, texID);
			}

			// Method should set default texture for each shape. Method must be overloaded!
			void unbind() const {
				m_texture.unbind();
			}

			/*
			Method should return id of texture associated to shape with specified index.
			Method must be overloaded!

				\param index - index of shape
				\return m_id[index] - id of texture prescribed to shape
			*/
			const uint32_t& getID(const uint32_t& index) const { 
				return m_texture.getID(index);
			}

			/*
			Method should return path to texture associated to shape with specified index.
			Method must be overloaded!

				\param index - index of path
				\return m_path[index] - path of texture prescribed to shape
			*/
			const std::string& getPath(const uint32_t& index) const { 
				return m_texture.getPath(index);
			}

			/*
			Method should add specified item to id vector. Must be overloaded!

				\param id - new id
			*/
			void addID(const uint32_t id) { 
				m_texture.addID(id);
			}

			/*
			Method should remove id prescribed to shape with specified index. Must be overloaded!

				\param index - index of shape
			*/
			void removeID(const uint32_t& index) {
				m_texture.removeID(index);
			}

			/*
			Method should reset static variable - texture unit.
			*/
			void resetTextureUnit() { 
				m_texture.resetTextureUnit();
			}
		};


} }

#endif // !TEXTURE_H
