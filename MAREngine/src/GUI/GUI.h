/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef GUI_H
#define GUI_H

#include "../mar.h"
#include "../Window/Window.h"

namespace mar {
	class GUI {
		Window* _window;
		const char* _versionGLSL;
		// --- Attributes for objects
		glm::vec3 _translation;
		glm::vec3 _angle;
		float _colors[4];
		std::vector<glm::vec3> _centersOfObjects;
		std::vector<glm::vec3> _angles;
		unsigned int _index;

	public:
		GUI(Window* window, const char* glsl_version);
		~GUI();

		void push(const glm::vec3& newCenter, const glm::vec3& newAngle);
		void prepareNewFrame();
		void display();

		const glm::mat4 getTranslationMatrix() const;
		const glm::mat4 getRotationMatrix() const;
		const float* getColors() const { return _colors; }
		const std::vector<glm::vec3>& getCentersVector() const;
		const std::vector<glm::vec3>& getAnglesVector() const;
		
	};
}

#endif // !GUI_H