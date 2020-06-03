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
			std::vector<glm::vec3> centers;
			std::vector<glm::vec3> angles;

			GUIData() = default;
		};

		class GUI {
			// --- Must-have to run GUI
			window::Window* m_window;
			// --- Attributes for sliders
			glm::vec3 m_sceneTranslation;
			glm::vec3 m_sceneAngle;
			float m_sceneColors[4];
			float m_shapePos[3];
			float m_shapeAngle[3];
			float m_inputCenter[3];
			// --- Run-time GUI attributes
			graphics::Mesh* global_mesh;
			const graphics::RendererStatistics* m_statistics;
			GUIData m_guiData;
			bool m_canModifyObjects;

		public:
			GUI();

			void initialize(window::Window* window, const char* glsl_version, bool can_modify_objects);
			void shutdown();
			
			void loadSceneParameters(graphics::Scene* scene);

			
			void prepareNewFrame();
			void updateSceneInfo(graphics::Mesh* mesh, const graphics::RendererStatistics* stats);
			void display();

		private:
			void display_GeneralMenu();
			void eventOnScene();
			void eventOnEachObjectSeperately();

			void display_ManageObjectsMenu();
			void addNewObjectToScene();
			void deleteObjectFromScene();

			void display_StatisticsMenu();

			void pushData(const glm::vec3& new_center, const glm::vec3& new_angle);
			void popData(const unsigned int& index);

		public:
			const glm::mat4 getTranslationMatrix() const;
			const glm::mat4 getRotationMatrix() const;
			const GUIData& getGUIData() const { return m_guiData; }
		};


} }

#endif // !MAR_ENGINE_GUI_H