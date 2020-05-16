/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef SCENE_H
#define SCENE_H

#include "../../mar.h"
#include "../RendererFactory.h"

namespace mar {

	enum class SceneType {
		DEFAULT,
		SURFACE,
		WITH_COLOURED_ELEMS,
		EMPTY
	};

	//! Scene
	/*!
		Scene contains everything needed to create basic and default scene
		on start of MAREngine.
	*/
    class Scene {
		std::vector<std::shared_ptr<Shape>> _shapes;
		std::vector<glm::vec3> _centers;
		std::vector<glm::vec3> _angles;
		std::vector<std::string> _textures;

    public:
		Scene() = default;

		void initializeScene(SceneType type = SceneType::DEFAULT) {
			if (type == SceneType::DEFAULT)
				createDefault();
			else if (type == SceneType::SURFACE)
				createSurface();
			else if (type == SceneType::WITH_COLOURED_ELEMS)
				createWithColouredElements();
			else if (type == SceneType::EMPTY)
				createEmpty();
		}

		void createDefault();
		void createSurface();
		void createWithColouredElements();
		void createEmpty();

		std::vector<std::shared_ptr<Shape>> getShapes() { return _shapes; }
		std::vector<glm::vec3> getCenters() { return _centers; }
		std::vector<glm::vec3> getAngles() { return _angles; }
		std::vector<std::string> getTextures() { return _textures; }

		const unsigned int getShapesNumber() { return _shapes.size(); }

		std::shared_ptr<Shape>& getShape(unsigned int index) { return _shapes[index]; }
		glm::vec3& getCenter(unsigned int index) { return _centers[index]; }
		glm::vec3& getAngle(unsigned int index) { return _angles[index]; }
		std::string& getTexture(unsigned int index) { return _textures[index]; }
    };

}

#endif // !SCENE_H