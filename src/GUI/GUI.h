/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_GUI_H
#define MAR_ENGINE_GUI_H

#include "../mar.h"
#include "../Renderer/Renderer.h"
#include "../Window/Window.h"
#include "../Renderer/Mesh/Mesh.h"
#include "../Renderer/Mesh/MeshCreator.h"
#include "../Renderer/Mesh/Shapes/DefaultShapes/Cube.h"
#include "../Renderer/Mesh/Shapes/DefaultShapes/Wall.h"
#include "../Renderer/Mesh/Shapes/DefaultShapes/Pyramid.h"
#include "../Renderer/Mesh/Shapes/DefaultShapes/Surface.h"

namespace mar {
	namespace gui {


		struct GUIData {
			float colors[4];
			glm::mat4 rotation;
			glm::mat4 translate;

			GUIData() = default;
		};

		class GUI {
			// --- Must-have to run GUI
			window::Window* m_window;
			// --- Attributes for sliders
			glm::vec3 m_sceneTranslation;
			glm::vec3 m_sceneAngle;
			float m_sceneColors[4];
			float m_inputCenter[3];
			// --- Run-time GUI attributes
			graphics::Mesh* global_mesh;
			const graphics::RendererStatistics* m_statistics;
			bool m_canModifyObjects;
			static GUIData s_guiData;

			bool m_displayGeneralScene{ false };
			bool m_displaySeperatelyShapes{ false };
			bool m_displayShapePush{ false };
			bool m_displayShapePop{ false };
			bool m_displayStatistics{ false };

		public:
			GUI();

			void initialize(window::Window* window, const char* glsl_version, bool can_modify_objects);
			void shutdown();
			
			void prepareNewFrame();
			void setReferences(graphics::Mesh* mesh, const graphics::RendererStatistics* stats);
			void display();

		private:
			void eventOnScene();
			void eventOnEachObjectSeperately();

			void addNewObjectToScene();
			void deleteObjectFromScene();

			void display_StatisticsMenu();

		public:
			const glm::mat4 getTranslationMatrix() const;
			const glm::mat4 getRotationMatrix() const;
			const static GUIData& getGUIData() { return s_guiData; }
		};


} }

#endif // !MAR_ENGINE_GUI_H