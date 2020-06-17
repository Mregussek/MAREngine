/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "Scene.h"

namespace mar {
	namespace graphics {


		void Scene::createDefault() {
			m_shapes = {
				MeshCreator::createCube()
				, MeshCreator::createCube()
				, MeshCreator::createPyramid()
				, MeshCreator::createSurface()
				, MeshCreator::createWall()
				, MeshCreator::createCube()
				, MeshCreator::createPyramid()
				, MeshCreator::createCube()
				, MeshCreator::createWall()
			};
			m_centers = {
				{ -2.0f,  0.0f,  -4.0f }
				, { -2.0f,  2.0f, -4.0f }
				, {  3.5f,  0.0f, -5.5f }
				, {  0.0f,  -0.025f,  0.0f }
				, { -10.0f,  0.0f,  0.0f }
				, { -7.5f,  0.0f, -7.5f }
				, { -5.5f,  0.0f, -6.5f }
				, {  1.25f,  0.0f,  -6.5f }
				, { -0.5f,  0.0f,  -10.5f }
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
				MeshCreator::createCube() ,
				MeshCreator::createCube() ,
				MeshCreator::createCube() ,
				MeshCreator::createCube() ,
				MeshCreator::createCube() ,
				MeshCreator::createCube() ,
				MeshCreator::createCube() ,
				MeshCreator::createCube() ,
				MeshCreator::createCube() ,
				MeshCreator::createCube() ,
				MeshCreator::createCube()
			};
			m_centers = {
				{ 5.0f,  0.0f, -2.0f } ,
				{ 7.0f,  0.0f, -2.0f } ,
				{ 9.0f,  0.0f, -2.0f } ,
				{ 11.0f, 0.0f, -2.0f } ,
				{ 5.0f,  2.0f, -2.0f } ,
				{ 7.0f,  2.0f, -2.0f } ,
				{ 9.0f,  2.0f, -2.0f } ,
				{ 11.0f, 2.0f, -2.0f } ,
				{ 13.0f, 0.0f, -2.0f } ,
				{ 13.0f, 0.0f, -4.0f } ,
				{ 13.0f, 2.0f, -4.0f } ,
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

		void Scene::createObjects() {
			m_shapes = {
				MeshCreator::createEmptyShape() 
				, MeshCreator::createEmptyShape() 
				//, MeshCreator::createEmptyShape() 
			};
			m_centers = {
				{  0.0f, -1.8f, -2.0f } ,
				{  2.0f, -1.8f, -2.0f } ,
				{ -2.0f,  0.0f,  1.0f } 
			};
			m_angles = {
				{ 270.f,  0.f, 90.f } ,
				{ 270.f,  0.f,  0.f } ,
				{ 270.f,  0.f, 90.f }
			};
			m_textures = {
				"empty" ,
				"empty" ,
				"empty" 
			};
			m_obj = {
				"resources/objects/lego-human.obj" 
				, "resources/objects/lego-human.obj" 
				, "resources/objects/low_poly_tree_triangles.obj"
			};
		}


} }