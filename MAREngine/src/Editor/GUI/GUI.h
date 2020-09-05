/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_EDITOR_GUI_H
#define MAR_ENGINE_EDITOR_GUI_H

#include "../../mar.h"
#include "../EditorLogging.h"

#include "../../Window/Window.h"

#include "../../Platform/OpenGL/FrameBufferOpenGL.h"
#include "../../Core/graphics/Renderer/RendererBatch.h"

#include "../../Core/ecs/Scene.h"
#include "../../Core/ecs/SceneManager.h"

#include "GUI_EntityPanel.h"
#include "GUI_EntityCollectionPanel.h"
#include "GUI_Filesystem.h"
#include "GUI_Info.h"
#include "GUI_Theme.h"


namespace mar {
	namespace editor {


		class GUI {
		public:
			GUI() = default;

			void initialize(const char* glsl_version);
			void shutdown();

			void submit(ecs::SceneManager* scene);

			void display();

			// --- GET METHODS --- //
			float getViewportWidth() { return m_viewportFramebuffer.getSpecification().width; }
			float getViewportHeight() { return m_viewportFramebuffer.getSpecification().height; }
			float getMouseViewportPosX() { return m_mouseViewportX; }
			float getMouseViewportPosY() { return m_mouseViewportY; }
			bool isViewportInputEnabled() { return m_enableViewportInput; }
			platforms::FrameBufferOpenGL& getFramebuffer() { return m_viewportFramebuffer; }
			ecs::Entity* getCurrentEntity();
			bool canDrawLines() { return m_sceneManager->isEditorMode() && m_sceneManager->useEditorCamera; }

		private:
			// --- DISPLAY --- //
			void prepareNewFrame();
			void updateFrame();
			void endFrame();

			// --- EDITOR --- //
			void Editor_MainMenuBar();
			void Editor_ViewPort();
			void Editor_Properties();

		private:
			// --- Storage for scenes
			ecs::SceneManager* m_sceneManager;
			// --- Viewport
			platforms::FrameBufferOpenGL m_viewportFramebuffer;
			float m_mouseViewportX{ 0.f };
			float m_mouseViewportY{ 0.f };
			bool m_enableViewportInput{ false };
			// --- Dockspace
			static bool s_dockspaceOpen;
			static bool s_fullscreenPersisant;
			// --- Should Window Be Opened ?
			bool m_newSceneWindow{ false };
			bool m_saveSceneWindow{ false };
			bool m_loadSceneWindow{ false };
			bool m_loadOBJfileWindow{ false };
			bool m_infoWindow{ false };
			bool m_instructionWindow{ false };
		};


} }

#endif // !MAR_ENGINE_GUI_H