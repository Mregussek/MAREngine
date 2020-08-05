/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_GUI_H
#define MAR_ENGINE_GUI_H

#include "../../mar.h"
#include "../EditorLogging.h"
#include "../Camera/Camera.h"

#include "../../Engine.h"
#include "../../Window/Window.h"

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
			graphics::FrameBufferOpenGL m_framebuffer;
			// --- Which entity should be displayed
			int32_t m_indexEntity{ -1 };
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

			// --- GET METHODS --- //
			const float& getViewportWidth() const { return m_framebuffer.getSpecification().width; }
			const float& getViewportHeight() const { return m_framebuffer.getSpecification().height; }
			graphics::FrameBufferOpenGL& getFramebuffer() { return m_framebuffer; }

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

			void Scene_Handle_TagComponent(bool& window_focused);
			void Scene_Handle_RenderableComponent(bool& window_focused);
			void Scene_Handle_TransformComponent(bool& window_focused);
			void Scene_Handle_ScriptComponent(bool& window_focused);
			void Scene_Handle_CameraComponent(bool& window_focused);
			void Scene_Handle_ColorComponent(bool& window_focused);
			void Scene_Handle_Texture2DComponent(bool& window_focused);
			void Scene_Handle_TextureCubemapComponent(bool& window_focused);
			void Scene_Handle_LightComponent(bool& window_focused);
		
			void Scene_Statistics();

			// --- FILESYSTEM HANDLERS --- //
			void Filesystem_SaveScene();
			void Filesystem_LoadScene();

			// --- TEXT EDITOR HANDLERS --- //
			void Display_Text(const char* text);
		};


} }

#endif // !MAR_ENGINE_GUI_H