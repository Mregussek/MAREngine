/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef SCENE_H
#define SCENE_H

#include "../../mar.h"
#include "../RendererFactory.h"

namespace mar {

	//! Scene
	/*!
		Scene contains everything needed to create basic and default scene
		on start of MAREngine.
	*/
    class Scene {
        std::vector<Shapes> _shapes = {
			Cube()
			, Pyramid()
			, Surface()
			, Wall()
			, Cube()
			, Pyramid()
		};
		std::vector<glm::vec3> _centers = {
			{ 0.0f,  0.0f,  0.0f }
			, { 3.0f,  0.0f, -4.5f }
			, { 0.0f,  -0.025f,  0.0f }
			, { -4.0f,  0.0f,  0.0f }
			, {-3.0f,  0.0f, -4.5f }
			, {-1.5f,  0.0f, -2.5f }
		};
		std::vector<glm::vec3> _angles = {
			 { 0.0f, 0.0f, 0.0f }
			 , { 0.0f, 0.0f, 0.0f }
			 , { 0.0f, 0.0f, 0.0f }
			 , { 0.0f, 0.0f, 0.0f }
			 , { 0.0f, 0.0f, 0.0f }
			 , { 0.0f, 0.0f, 0.0f }
		};
		std::vector<std::string> _textures = {
			TexturePaths.mrTex
			, TexturePaths.yellowTex
			, TexturePaths.grassTex
			, TexturePaths.wallTex
			, TexturePaths.blueTex
			, TexturePaths.redTex
		};

    public:
		Scene() = default;

		std::vector<Shapes> getShapes() { return _shapes; }

		std::vector<glm::vec3> getCenters() { return _centers; }

		std::vector<glm::vec3> getAngles() { return _angles; }

		std::vector<std::string> getTextures() { return _textures; }
    };

}

#endif // !SCENE_H