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
		class Texture : std::enable_shared_from_this<Texture> {
		public:

			// This method should deleting whole stuff associated to Textures.
			// Method must be overloaded!
			virtual void shutdown() { }

			/*
			Method should generate new id for texture. Must be overloaded!

				\param path - path to texture, which will be loaded
				\return id - id for new loaded texture
			*/
			virtual unsigned int genNewTexture(const char* path) { return 0; }

			/*
			Method should load 2D texture and prescribe it to available index.
			Method must be overloaded!

				\param path - path to texture, which will be loaded
			*/
			virtual void loadTexture(const char* path) { }

			/*
			Method should generate new id for cubemap. Must be overloaded!

				\param faces - paths to textures, which will be loaded
				\return id - id for new loaded cubemap
			*/
			virtual unsigned int genNewCubemap(const char* path) { return 0; }

			/*
			Method should load Cube Map and prescribe it to available index.
			Method must be overloaded!

				\param faces - paths to textures, which will be loaded
			*/
			virtual void loadCubemap(const char* path) { }

			/*
			Method should bind texture (with unique ID) to selected shape (with unique ID)
			Method must be overloaded!

				\param shapeId - id of shape
				\param texID - id of texture
			*/
			virtual void bind(const int& shapeId, const unsigned int& texID) const { }

			// Method should set default texture for each shape. Method must be overloaded!
			virtual void unbind() const { }

			/*
			Method should return id of texture associated to shape with specified index.
			Method must be overloaded!

				\param index - index of shape
				\return m_id[index] - id of texture prescribed to shape
			*/
			virtual const unsigned int& getID(const unsigned int& index) const { return 0; }

			/*
			Method should return path to texture associated to shape with specified index.
			Method must be overloaded!

				\param index - index of path
				\return m_path[index] - path of texture prescribed to shape
			*/
			virtual const std::string& getPath(const unsigned int& index) const { return "empty"; }

			/*
			Method should add specified item to id vector. Must be overloaded!

				\param id - new id
			*/
			virtual void addID(const unsigned int id) { }

			/*
			Method should remove id prescribed to shape with specified index. Must be overloaded!

				\param index - index of shape
			*/
			virtual void removeID(const unsigned int& index) { }
		};


} }

#endif // !TEXTURE_H
