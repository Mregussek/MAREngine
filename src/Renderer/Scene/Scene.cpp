/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "Scene.h"

namespace mar {
	namespace graphics {


		void Scene::createDefault() {
			m_shapes = {
				std::make_shared<Cube>()
				, std::make_shared<Pyramid>()
				, std::make_shared<Surface>()
				, std::make_shared<Wall>()
				, std::make_shared<Cube>()
				, std::make_shared<Pyramid>()
			};
			m_centers = {
				{ 0.0f,  0.0f,  0.0f }
				, { 3.0f,  0.0f, -4.5f }
				, { 0.0f,  -0.025f,  0.0f }
				, { -4.0f,  0.0f,  0.0f }
				, {-3.0f,  0.0f, -4.5f }
				, {-1.5f,  0.0f, -2.5f }
			};
			m_angles = {
				 { 0.0f, 0.0f, 0.0f }
				 , { 0.0f, 0.0f, 0.0f }
				 , { 0.0f, 0.0f, 0.0f }
				 , { 0.0f, 0.0f, 0.0f }
				 , { 0.0f, 0.0f, 0.0f }
				 , { 0.0f, 0.0f, 0.0f }
			};
			m_textures = {
				TexturePaths.mrTex
				, TexturePaths.yellowTex
				, TexturePaths.grassTex
				, TexturePaths.wallTex
				, TexturePaths.blueTex
				, TexturePaths.redTex
			};
		}

		void Scene::createWithColouredElements() {
			m_shapes = {
				std::make_shared<Cube>()
				, std::make_shared<Cube>()
				, std::make_shared<Pyramid>()
				, std::make_shared<Surface>()
				, std::make_shared<Wall>()
				, std::make_shared<Cube>()
				, std::make_shared<Pyramid>()
				, std::make_shared<Cube>()
				, std::make_shared<Wall>()
			};
			m_centers = {
				{ 0.0f,  0.0f,  0.0f }
				, { 0.0f,  2.0f,  0.0f }
				, { 5.0f,  0.0f, -4.5f }
				, { 0.0f,  -0.025f,  0.0f }
				, { -6.0f,  0.0f,  0.0f }
				, {-4.5f,  0.0f, -4.5f }
				, {-2.5f,  0.0f, -2.5f }
				, { 2.5f,  0.0f,  -3.5f }
				, { 3.5f,  0.0f,  -10.5f }
			};
			m_angles = {
				 { 0.0f, 0.0f, 0.0f }
				 , { 0.0f, 0.0f, 0.0f }
				 , { 0.0f, 0.0f, 0.0f }
				 , { 0.0f, 0.0f, 0.0f }
				 , { 0.0f, 0.0f, 0.0f }
				 , { 0.0f, 0.0f, 0.0f }
				 , { 0.0f, 0.0f, 0.0f }
				 , { 0.0f, 0.0f, 0.0f }
				 , { 0.0f, 90.0f, 0.0f }
			};
			m_textures = {
				"empty"
				, TexturePaths.mrTex
				, TexturePaths.blueTex
				, TexturePaths.grassTex
				, TexturePaths.wallTex
				, "empty"
				, "empty"
				, TexturePaths.blackTex
				, TexturePaths.wallTex
			};
		}

		void Scene::createSecondMesh() {
			m_shapes = {
				std::make_shared<Cube>() ,
				std::make_shared<Cube>() ,
				std::make_shared<Cube>() ,
				std::make_shared<Cube>() ,
				std::make_shared<Cube>() ,
				std::make_shared<Cube>() ,
				std::make_shared<Cube>() ,
				std::make_shared<Cube>() ,
				std::make_shared<Cube>() ,
				std::make_shared<Cube>() ,
				std::make_shared<Cube>()
			};
			m_centers = {
				{ 5.0f,  0.0f, 0.0f } ,
				{ 7.0f,  0.0f, 0.0f } ,
				{ 9.0f,  0.0f, 0.0f } ,
				{ 11.0f, 0.0f, 0.0f } ,
				{ 5.0f,  2.0f, 0.0f } ,
				{ 7.0f,  2.0f, 0.0f } ,
				{ 9.0f,  2.0f, 0.0f } ,
				{ 11.0f, 2.0f, 0.0f } ,
				{ 13.0f, 0.0f, 0.0f } ,
				{ 13.0f, 0.0f, -2.0f } ,
				{ 13.0f, 2.0f, -2.0f } ,
			};
			m_angles = {
				 { 0.0f, 0.0f, 0.0f }
				 , { 0.0f, 0.0f, 0.0f }
				 , { 0.0f, 0.0f, 0.0f }
				 , { 0.0f, 0.0f, 0.0f }
				 , { 0.0f, 0.0f, 0.0f }
				 , { 0.0f, 0.0f, 0.0f }
				 , { 0.0f, 0.0f, 0.0f }
				 , { 0.0f, 0.0f, 0.0f }
				 , { 0.0f, 0.0f, 0.0f }
				 , { 0.0f, 0.0f, 0.0f }
				 , { 0.0f, 0.0f, 0.0f }
			};
			m_faces = {
				TexturePaths.mc_grass_faces ,
				TexturePaths.mc_grass_faces ,
				TexturePaths.mc_grass_faces ,
				TexturePaths.mc_grass_faces ,
				TexturePaths.mc_grass_faces ,
				TexturePaths.mc_grass_faces ,
				TexturePaths.mc_cobble_faces ,
				TexturePaths.mc_cobble_faces ,
				TexturePaths.mc_cobble_faces ,
				TexturePaths.mc_diamond_faces ,
				TexturePaths.mc_diamond_faces
			};
		}

		void Scene::createSurface() {
			m_shapes = {
				std::make_shared<Surface>()
			};
			m_centers = {
				{ 0.0f,  -0.025f,  0.0f }
			};
			m_angles = {
				 { 0.0f, 0.0f, 0.0f }
			};
			m_textures = {
				TexturePaths.grassTex
			};
		}


} }