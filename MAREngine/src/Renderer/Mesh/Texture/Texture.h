/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef TEXTURE_H
#define TEXTURE_H

#include "../../../mar.h"

namespace mar {

	struct {
#ifndef IMPORTED_FROM_GITHUB 
		const std::string mrTex = "resources/textures/mr.jpg";
		const std::string yellowTex = "resources/textures/yellow-texture.jpg";
		const std::string grassTex = "resources/textures/grass-texture.jpg";
		const std::string wallTex = "resources/textures/wall.jpg";
		const std::string blueTex = "resources/textures/blue-texture.jpg";
		const std::string redTex = "resources/textures/red-texture.jpg";
		const std::string blackTex = "resources/textures/blac-texture.jpg";
#else
		const std::string mrTex =		"../MAREngine/resources/textures/mr.jpg";
		const std::string yellowTex =	"../MAREngine/resources/textures/yellow-texture.jpg";
		const std::string grassTex =	"../MAREngine/resources/textures/grass-texture.jpg";
		const std::string wallTex =		"../MAREngine/resources/textures/wall.jpg";
		const std::string blueTex =		"../MAREngine/resources/textures/blue-texture.jpg";
		const std::string redTex =		"../MAREngine/resources/textures/red-texture.jpg";
		const std::string blackTex =	"../MAREngine/resources/textures/blac-texture.jpg";
#endif
	} TexturePaths;


	//! Texture
	/*!
		Texture is used to bind textures to renderer objects.
		This class expects, that every renderer shape is containing unique ID, which 
		will be prescribed to texture ID.
		This is just base class for other implementations of Texture.
	*/
	class Texture : std::enable_shared_from_this<Texture> {
	public:
		//! This method should deleting whole stuff associated to Textures.
		//! Method must be overloaded!
		virtual void shutdown() { }

		//! Method should load 2D texture and prescribe it to available index.
		//! Method must be overloaded!
		/*
			\param path - path to texture, which will be loaded
		*/
		virtual void loadTexture(const std::string& path) { }

		//! Method should bind texture (with unique ID) to selected shape (with unique ID)
		//! Method must be overloaded!
		/*
			\param shapeId - id of shape
			\param texID - id of texture
		*/
		virtual void bind(const float& shapeId, const unsigned int& texID) const { }

		//! Method should set default texture for each shape. Method must be overloaded!
		virtual void unbind() const { }

		//! Method should return id of texture associated to shape with specified index.
		//! Method must be overloaded!
		/*
			\param index - index of shape
			\return _id[index] - id of texture prescribed to shape
		*/
		virtual const unsigned int& getID(int index) const { return unsigned int(0); }

		//! Method should removes id prescribed to shape with specifed index. Must be overloaded!
		/*
			\param index - index of shape
		*/
		virtual void removeID(const unsigned int& index) { }

		//! Method should return width of last loaded texture
		/*
			\return _width - width of last loaded texture
		*/
		virtual const int& getWidth() const { return int(-1); }

		//! Method should return height of last loaded texture
		/*
			\return _height - height of last loaded texture
		*/
		virtual const int& getHeight() const { return int(-1); }
	};
}

#endif // !TEXTURE_H
