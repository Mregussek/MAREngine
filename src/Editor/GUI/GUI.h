/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_GUI_H
#define MAR_ENGINE_GUI_H

#include "../FileSystem/filesystem.h"
#include "../../mar.h"
#include "../../Debug/Log.h"
#include "../../MAREngine.h"
#include "../../Window/Window.h"
#include "../../Core/Mesh/MeshCreator.h"
#include "../../Core/Light/Light.h"
#include "../../Core/Renderer/RendererOpenGL.h"
#include "../../Core/Renderer/Buffers/FrameBuffer/FrameBuffer.h"
#include "../../Core/Renderer/Buffers/FrameBuffer/FrameBufferOpenGL.h"


#include "../../Core/Renderer/RendererEntity.h"
#include "../../Core/Scene/Scene.h"
#include "../../Core/Scene/Entity/Entity.h"
#include "../../Core/Scene/Component/Components.h"
#include "../../Core/Scene/System/Systems.h"
#include "../../Core/Camera/Camera.h"


namespace mar {
	namespace editor {

		struct scene {
			static std::vector<const char*> files;
			static int selected;
		};

		struct texture {
			static std::vector<const char*> files;
			static int selected;
		};
		
		struct GUIData {
			float colors[4];
			maths::mat4 rotation;
			maths::mat4 translate;

			GUIData() = default;
		};

		class GUI {
			// --- Attributes for sliders && input
			maths::vec3 m_sceneTranslation{ 0.0f, 0.0f, 0.0f };
			maths::vec3 m_sceneAngle{ 0.0f, 0.0f, 0.0f };
			float m_sceneColors[4];
			float m_inputCenter[3];
			float m_generalScale{ 1.f };
			char m_inputStr[20];
			// --- Run-time GUI attributes
			std::vector<graphics::Mesh*> m_meshes;
			std::vector<const char*> m_meshesNames;
			graphics::Light* m_light;
			static GUIData s_guiData;
			int m_meshIndex;
			int m_shapeIndex;
			int m_pushMeshIndex;
			// --- Dockspace
			graphics::FrameBuffer<graphics::FrameBufferOpenGL> m_framebuffer;
			static bool s_dockspaceOpen;
			static bool s_fullscreenPersisant;
			bool m_infoWindow{ false };
			bool m_instructionWindow{ false };
			bool m_fileOpenWindow{ false };
			bool m_fileSaveWindow{ false };

		public:
			GUI() = default;

			void initialize(const char* glsl_version);
			void shutdown();

			void prepareNewFrame();
			void display();

			void submit(graphics::Mesh* mesh) { 
				m_meshes.push_back(mesh);
				m_meshesNames.push_back(mesh->getMeshName());
			}

			// --- SET METHODS --- //
			void set(const graphics::FrameBuffer<graphics::FrameBufferOpenGL>& framebuffer) { m_framebuffer = framebuffer; }
			void set(graphics::Light* light) { m_light = light; }

			// --- GET METHODS --- //
			const maths::mat4 getTranslationMatrix() const;
			const maths::mat4 getRotationMatrix() const;
			inline const static GUIData& getGUIData() { return s_guiData; }

		private:
			void Setup_Theme();

			void File_Open();
			void File_Save();

			void Display_ViewPort();
			/*
			void Menu_ModifyScene();
			void Menu_ModifyShape();
			void Menu_SelectShape();
			void Menu_PushShapeToScene();
			*/
			void Scene_Environment();
			void Scene_Statistics();
			void Menu_Info();
			void Menu_Instruction();

			bool checkCharsEnding(const char* withwhat, const char* what);
			bool checkCharsStart(const char* withwhat, const char* what);


		public:
			std::vector<ecs::Scene*> m_scenes;
			int32_t index_scene{ -1 };
			int32_t index_entity{ -1 };
			void submit(ecs::Scene* scene);
			void Scene_Hierarchy();
			void Scene_Entity_Modify();

			void Scene_Handle_TagComponent();
			void Scene_Handle_TransformComponent();
			void Scene_Handle_ColorComponent();
			void Scene_Handle_LightComponent();
		};


} }

#endif // !MAR_ENGINE_GUI_H