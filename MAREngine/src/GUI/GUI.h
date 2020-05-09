/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef GUI_H
#define GUI_H

#include "../mar.h"
#include "../Renderer/Renderer.h"
#include "../Window/Window.h"

namespace mar {

	class GUI {
		// --- Must-have to run GUI
		Window* _window;
		const char* _versionGLSL;
		// --- Attributes for objects
		glm::vec3 _translation;
		glm::vec3 _angle;
		float _colors[4];
		std::vector<glm::vec3> _centersOfObjects;
		std::vector<glm::vec3> _angles;
		unsigned int _index;
		// --- Run-time GUI attributes
		Renderer* _renderer;
		bool _rendererConnected;
		float _pos[3];
		float _ang[3];
		float _inputCenter[3];
		bool _checkPyramid;
		bool _checkCube;
		bool _checkSurface;

	public:
		GUI(Window* window, const char* glsl_version);
		~GUI();

		void connectToRenderer(Renderer* renderer);

		void push(const glm::vec3& newCenter, const glm::vec3& newAngle);
		void prepareNewFrame();
		void display();

		void eventOnScene();
		void eventOnEachObjectSeperately();
		void addNewObjectToScene();
		void deleteObjectFromScene();

		const glm::mat4 getTranslationMatrix() const;
		const glm::mat4 getRotationMatrix() const;
		const float* getColors() const { return _colors; }
		const std::vector<glm::vec3>& getCentersVector() const;
		const std::vector<glm::vec3>& getAnglesVector() const;
		
	};
}

#endif // !GUI_H