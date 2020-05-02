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

	public:
		GUI(Window* window, const char* glsl_version);
		~GUI();

		const glm::mat4 getTranslationMatrix() const { return glm::translate(glm::mat4(1.0f), _translation); }
		const glm::mat4 getRotationMatrix() const { 
			glm::mat4 rotation = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
			return glm::rotate(rotation, glm::radians(_angle.x), glm::vec3(1.0f, 0.0f, 0.0f)) 
				* glm::rotate(rotation, glm::radians(_angle.y), glm::vec3(0.0f, 1.0f, 0.0f))
				* glm::rotate(rotation, glm::radians(_angle.z), glm::vec3(0.0f, 0.0f, 1.0f));
		}
		const float* getColors() const { return _colors; }

		void prepareNewFrame();
		void display();
	};
}

#endif // !GUI_H