/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef TEXTURE_H
#define TEXTURE_H

#include "../../../mar.h"

namespace mar {
	namespace graphics {


		struct {
#ifndef IMPORTED_FROM_GITHUB 
			const std::string mrTex = "resources/textures/mr.jpg";
			const std::string yellowTex = "resources/textures/yellow-texture.jpg";
			const std::string grassTex = "resources/textures/grass-texture.jpg";
			const std::string wallTex = "resources/textures/wall.jpg";
			const std::string blueTex = "resources/textures/blue-texture.jpg";
			const std::string redTex = "resources/textures/red-texture.jpg";
			const std::string blackTex = "resources/textures/black-texture.jpg";

			const std::vector<std::string> SkyboxFaces {
				"resources/textures/skybox/right.jpg" , 
				"resources/textures/skybox/left.jpg" ,
				"resources/textures/skybox/top.jpg" ,
				"resources/textures/skybox/bottom.jpg" ,
				"resources/textures/skybox/front.jpg" ,
				"resources/textures/skybox/back.jpg" ,
			};

			const std::vector<std::string> mc_grass_faces{
				 "resources/textures/mc/mc-grass.jpg"
				 ,  "resources/textures/mc/mc-grass.jpg"
				 ,  "resources/textures/mc/mc-grass-up.jpg"
				 ,  "resources/textures/mc/dirt.jpg"
				 ,  "resources/textures/mc/mc-grass.jpg"
				 ,  "resources/textures/mc/mc-grass.jpg"
			};

			const std::vector<std::string> mc_cobble_faces{
				"resources/textures/mc/mc-cobble.jpg"
				, "resources/textures/mc/mc-cobble.jpg"
				, "resources/textures/mc/mc-cobble.jpg"
				, "resources/textures/mc/mc-cobble.jpg"
				, "resources/textures/mc/mc-cobble.jpg"
				, "resources/textures/mc/mc-cobble.jpg"
			};

#else
			const std::string mrTex = "../../../resources/textures/mr.jpg";
			const std::string yellowTex = "../../../resources/textures/yellow-texture.jpg";
			const std::string grassTex = "../../../resources/textures/grass-texture.jpg";
			const std::string wallTex = "../../../resources/textures/wall.jpg";
			const std::string blueTex = "../../../resources/textures/blue-texture.jpg";
			const std::string redTex = "../../../resources/textures/red-texture.jpg";
			const std::string blackTex = "../../../resources/textures/black-texture.jpg";

			const std::vector<std::string> SkyboxFaces{
				"../../../resources/textures/skybox/right.jpg" ,
				"../../../resources/textures/skybox/left.jpg" ,
				"../../../resources/textures/skybox/top.jpg" ,
				"../../../resources/textures/skybox/bottom.jpg" ,
				"../../../resources/textures/skybox/front.jpg" ,
				"../../../resources/textures/skybox/back.jpg" ,
			};

			const std::vector<std::string> mc_grass_faces{
				 "../../../resources/textures/mc/mc-grass.jpg"
				 ,  "../../../resources/textures/mc/mc-grass.jpg"
				 ,  "../../../resources/textures/mc/mc-grass-up.jpg"
				 ,  "../../../resources/textures/mc/dirt.jpg"
				 ,  "../../../resources/textures/mc/mc-grass.jpg"
				 ,  "../../../resources/textures/mc/mc-grass.jpg"
		};

			const std::vector<std::string> mc_cobble_faces{
				"../../../resources/textures/mc/mc-cobble.jpg"
				, "../../../resources/textures/mc/mc-cobble.jpg"
				, "../../../resources/textures/mc/mc-cobble.jpg"
				, "../../../resources/textures/mc/mc-cobble.jpg"
				, "../../../resources/textures/mc/mc-cobble.jpg"
				, "../../../resources/textures/mc/mc-cobble.jpg"
			};
#endif
		} TexturePaths;


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
			virtual unsigned int genNewTexture(const std::string& path) { return 0; }

			/*
			Method should load 2D texture and prescribe it to available index.
			Method must be overloaded!

				\param path - path to texture, which will be loaded
			*/
			virtual void loadTexture(const std::string& path) { }

			/*
			Method should generate new id for cubemap. Must be overloaded!

				\param faces - paths to textures, which will be loaded
				\return id - id for new loaded cubemap
			*/
			virtual unsigned int genNewCubemap(const std::vector<std::string>& faces) { return 0; }

			/*
			Method should load Cube Map and prescribe it to available index.
			Method must be overloaded!

				\param faces - paths to textures, which will be loaded
			*/
			virtual void loadCubemap(const std::vector<std::string>& faces) { }

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
			virtual const unsigned int& getID(int index) const { return unsigned int(0); }

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
