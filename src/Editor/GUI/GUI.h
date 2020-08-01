/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_GUI_H
#define MAR_ENGINE_GUI_H

#include "../../mar.h"
#include "../EditorLogging.h"

#include "../../MAREngine.h"
#include "../../Window/Window.h"

#include "../../Core/graphics/Camera/Camera.h"
#include "../../Core/graphics/Renderer/Buffers/FrameBuffer/FrameBuffer.h"
#include "../../Core/graphics/Renderer/Buffers/FrameBuffer/FrameBufferOpenGL.h"
#include "../../Core/graphics/Renderer/RendererEntity.h"

#include "../../Core/ecs/Scene.h"
#include "../../Core/ecs/Entity.h"
#include "../../Core/ecs/Components.h"
#include "../../Core/ecs/Systems.h"


namespace mar {
	namespace editor {


		class GUI {
			// --- Storage for scenes
			ecs::Scene* m_scene;
			// --- Viewport
			graphics::FrameBuffer<graphics::FrameBufferOpenGL> m_framebuffer;
			// --- Which entity should be displayed
			int32_t m_indexEntity{ -1 };
			bool m_modifyRenderable{ false };
			// --- Dockspace
			static bool s_dockspaceOpen;
			static bool s_fullscreenPersisant;
			// --- Should Window Be Opened ?
			bool m_saveSceneWindow{ false };
			bool m_loadSceneWindow{ false };
			bool m_infoWindow{ false };
			bool m_instructionWindow{ false };

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
			void Scene_Hierarchy_PopUp();

			void Scene_Entity_Modify();
			void Scene_Entity_Modify_PopUp();

			void Scene_Handle_TagComponent();
			void Scene_Handle_RenderableComponent();
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