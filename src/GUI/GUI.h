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

	struct GUIData {
		float colors[4];
		glm::mat4 rotation;
		glm::mat4 translate;
		std::vector<glm::vec3> centers;
		std::vector<glm::vec3> angles;

		GUIData() = default;
	};

	class GUI {
	private:
		// --- Must-have to run GUI
		Window* _window;
		const char* _versionGLSL;
		// --- Attributes for objects
		glm::vec3 _translation;
		glm::vec3 _angle;
		// --- Attributes for sliders
		float _colors[4];
		float _pos[3];
		float _ang[3];
		float _inputCenter[3];
		// --- Run-time GUI attributes
		Renderer* _renderer;
		GUIData _guiData;
		bool _rendererConnected;
		unsigned int _startupSceneSize;
		
	public:
		GUI();

		void initialize(Window* window, const char* glsl_version);
		void shutdown();

		void loadSceneParameters(Scene* scene);

		void connectToRenderer(Renderer* renderer);

		void push(const glm::vec3& newCenter, const glm::vec3& newAngle);
		void prepareNewFrame();
		void display();

		void eventOnScene();
		void eventOnEachObjectSeperately();
		void addNewObjectToScene();
		void deleteObjectFromScene();
		void displayStatistics();

		const glm::mat4 getTranslationMatrix() const;
		const glm::mat4 getRotationMatrix() const;
		
		const GUIData& getGUIData() const { return _guiData; }
	};
}

#endif // !GUI_H