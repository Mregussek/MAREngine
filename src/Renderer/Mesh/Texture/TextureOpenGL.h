/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef TEXTURE_OPENGL_H
#define TEXTURE_OPENGL_H

#include "Texture.h"

namespace mar {

	//! TextureOpenGL
	/*!
		TextureOpenGL is used to bind textures to renderer objects.
		This class expects, that every renderer shape is containing unique ID, which
		will be prescribed to texture ID.
		This is overloaded class of base one - Texture, which implements 
		solutions to specified problems using OpenGL.
	*/
	class TextureOpenGL : public Texture, std::enable_shared_from_this<TextureOpenGL> {
		std::vector<unsigned int> _id;
		std::vector<std::string> _paths;
		unsigned int _idOfNextTexture{0};
		unsigned char* _localBuffer;
		int _width;
		int _height;
		int _bitPerPixel;

	public:
		//! Delete all existing textures
		void shutdown() override;

		//! Load 2D texture and prescribe it to available index. If texture is loaded
		//! it pushes its index again.
		/*
			\param path - path to texture, which will be loaded
		*/
		void loadTexture(const std::string& path) override;

		//! Bind texture with texID to selected shape with shapeId.
		/*
			\param shapeId - id of shape
			\param texID - id of texture
		*/
		void bind(const float& shapeId, const unsigned int& texID) const override;
		
		//! Set default texture for each shape
		void unbind() const override;

		//! Get id of texture bounded to shape on specific index
		/*
			\param index - index of shape
			\return _id[index] - id of texture prescribed to shape
		*/
		const unsigned int& getID(int index) const override;

		//! Removes id prescribed to shape with specifed index
		/*
			\param index - index of shape
		*/
		void removeID(const unsigned int& index) override;

		//! Get width of last loaded texture
		/*
			\return _width - width of last loaded texture
		*/
		const int& getWidth() const override;

		//! Get height of last loaded texture
		/*
			\return _height - height of last loaded texture
		*/
		const int& getHeight() const override;
	};
}

#endif // !TEXTURE_OPENGL_H