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
		class TextureOpenGL : public Texture, std::enable_shared_from_this<TextureOpenGL> {
			std::vector<unsigned int> m_id;
			std::unordered_map<std::string, unsigned int> m_paths;

		public:

			// Delete all existing textures
			void shutdown() override;

			/*
			Method generates new id for texture.

				\param path - path to texture, which will be loaded
				\return id - id for new loaded texture
			*/
			unsigned int genNewTexture(const std::string& path) override;

			/*
			Load 2D texture and prescribe it to available index. If texture is loaded
			it pushes its index again.

				\param path - path to texture, which will be loaded
				\return id - id for new loaded texture
			*/
			void loadTexture(const std::string& path) override;

			/*
			Method generates new id for cubemap. Must be overloaded!

				\param faces - paths to textures, which will be loaded
				\return id - id for new loaded cubemap
			*/
			unsigned int genNewCubemap(const std::vector<std::string>& faces) override;

			/*
			Method loads Cube Map and prescribe it to available index.
			Method must be overloaded!

				\param faces - paths to textures, which will be loaded
			*/
			void loadCubemap(const std::vector<std::string>& faces) override;

			/*
			Bind texture with texID to selected shape with shapeId.

				\param shapeId - id of shape
				\param texID - id of texture
			*/
			void bind(const int& shapeId, const unsigned int& texID) const override;

			// Set default texture for each shape
			void unbind() const override;

			/*
			Get id of texture bounded to shape on specific index

				\param index - index of shape
				\return m_id[index] - id of texture prescribed to shape
			*/
			const unsigned int& getID(int index) const override;
 
			/*
			Adds id prescribed to shape

				\param id - new id
			*/
			void addID(const unsigned int id) override;

			/*
			Removes id prescribed to shape with specified index

				\param index - index of shape
			*/
			void removeID(const unsigned int& index) override;
		};


} }

#endif // !TEXTURE_OPENGL_H