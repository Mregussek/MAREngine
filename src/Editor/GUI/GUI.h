/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_GUI_H
#define MAR_ENGINE_GUI_H

#include "../../mar.h"
#include "../../Debug/Log.h"
#include "../../MAREngine.h"
#include "../../Window/Window.h"
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


		class GUI {
			// --- Dockspace
			static bool s_dockspaceOpen;
			static bool s_fullscreenPersisant;
			// --- Viewport
			graphics::FrameBuffer<graphics::FrameBufferOpenGL> m_framebuffer;
			// --- Should Window Be Opened ?
			bool m_saveSceneWindow{ false };
			bool m_loadSceneWindow{ false };
			bool m_infoWindow{ false };
			bool m_instructionWindow{ false };
			// --- Storage for scenes
			ecs::Scene* m_scene;
			// --- Which entity should be displayed
			int32_t index_scene{ -1 };
			int32_t index_entity{ -1 };

		public:
			GUI() = default;

			void initialize(const char* glsl_version);
			void shutdown();

			void submit(ecs::Scene* scene);

			void display();

			// --- SET METHODS --- //
			void set(const graphics::FrameBuffer<graphics::FrameBufferOpenGL>& framebuffer) { m_framebuffer = framebuffer; }

		private:
			// --- DISPLAY --- //
			void prepareNewFrame();
			void updateFrame();
			void endFrame();

			void Setup_Theme();

			void Display_ViewPort();
			
			void Menu_MainMenuBar();
			void Menu_Info();
			void Menu_Instruction();

			// --- SCENE HANDLERS --- //
			void Scene_Hierarchy();

			void Scene_Entity_Modify();

			void Scene_Handle_TagComponent();
			void Scene_Handle_TransformComponent();
			void Scene_Handle_ColorComponent();
			void Scene_Handle_LightComponent();
		
			void Scene_Statistics();

			// --- FILESYSTEM HANDLERS --- //
			void Filesystem_SaveScene();
			void Filesystem_LoadScene();
		};


} }

#endif // !MAR_ENGINE_GUI_H