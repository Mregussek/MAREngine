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

		struct GUITextureList {
			static const char* s_textures[];
			static int s_selectedItem;
		};

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
			glm::vec3 m_sceneTranslation{ 0.0f, 0.0f, 0.0f };
			glm::vec3 m_sceneAngle{ 0.0f, 0.0f, 0.0f };
			float m_sceneColors[4];
			float m_inputCenter[3];
			// --- Run-time GUI attributes
			std::vector<graphics::Mesh*> m_meshes;
			const graphics::RendererStatistics* m_statistics;
			bool m_canModifyObjects{ false };
			static GUIData s_guiData;

			bool m_modifySceneWindowDisplay{ false };
			bool m_infoWindow{ false };

		public:
			GUI() = default;

			void initialize(window::Window* window, const char* glsl_version, bool can_modify_objects);
			void shutdown();
			
			void prepareNewFrame();
			void display();

			void submitMesh(graphics::Mesh* mesh) { m_meshes.push_back(mesh); }

		private:
			void Menu_ModifyScene();
			void Menu_PushShapeToScene();
			void Menu_Statistics();
			void Menu_Info();

		public:
			// --- SET METHODS --- //
			void setReferences(const graphics::RendererStatistics* stats) { m_statistics = stats; }

			// --- GET METHODS --- //
			const glm::mat4 getTranslationMatrix() const;
			const glm::mat4 getRotationMatrix() const;
			inline const static GUIData& getGUIData() { return s_guiData; }
		};


} }

#endif // !MAR_ENGINE_GUI_H