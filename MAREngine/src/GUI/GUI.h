
#ifndef GUI_H
#define GUI_H

#include "../mar.h"
#include "../Window/Window.h"

namespace mar {
	class GUI {
		Window* _window;
		const char* _versionGLSL;
		glm::vec3 _translation;

	public:
		GUI(Window* window, const char* glsl_version);
		~GUI();

		const glm::mat4 getTranslationMatrix() const { return glm::translate(glm::mat4(1.0f), _translation); }

		void prepareNewFrame();
		void display();
	};
}

#endif // !GUI_H