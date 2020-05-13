/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "Scene.h"

namespace mar {
    void Scene::createDefault() {
		_shapes = {
			Cube()
			, Pyramid()
			, Surface()
			, Wall()
			, Cube()
			, Pyramid()
		};
		_centers = {
			{ 0.0f,  0.0f,  0.0f }
			, { 3.0f,  0.0f, -4.5f }
			, { 0.0f,  -0.025f,  0.0f }
			, { -4.0f,  0.0f,  0.0f }
			, {-3.0f,  0.0f, -4.5f }
			, {-1.5f,  0.0f, -2.5f }
		};
		_angles = {
			 { 0.0f, 0.0f, 0.0f }
			 , { 0.0f, 0.0f, 0.0f }
			 , { 0.0f, 0.0f, 0.0f }
			 , { 0.0f, 0.0f, 0.0f }
			 , { 0.0f, 0.0f, 0.0f }
			 , { 0.0f, 0.0f, 0.0f }
		};
		_textures = {
			TexturePaths.mrTex
			, TexturePaths.yellowTex
			, TexturePaths.grassTex
			, TexturePaths.wallTex
			, TexturePaths.blueTex
			, TexturePaths.redTex
		};
    }

	void Scene::createSurface() {
		_shapes = {
			Surface()
		};
		_centers = {
			{ 0.0f,  -0.025f,  0.0f }
		};
		_angles = {
			 { 0.0f, 0.0f, 0.0f }
		};
		_textures = {
			TexturePaths.grassTex
		};
	}

	void Scene::createEmpty() {
		_shapes.clear();
		_centers.clear();
		_angles.clear();
		_textures.clear();
	}
}