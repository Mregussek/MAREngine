/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_ECS_SCENE_MANAGER_H
#define MAR_ENGINE_ECS_SCENE_MANAGER_H


#include "../../mar.h"
#include "ECS/ComponentsEntity.h"
#include "Scene.h"


namespace mar {
	namespace graphics { class RenderPipeline; }

	namespace ecs {

		class SceneEvents;


		class SceneManager {
		// -------------------------------------------------------------
		// SCENE MANAGER PUBLIC METHODS
		// -------------------------------------------------------------

		public:
			SceneManager();
	
			void initialize();
			void shutdown();
			void update();

			void setScene(Scene* scene) { m_scene = scene; }
			Scene* getScene() { return m_scene; }

			bool isEditorMode() { return m_EditorMode; }
			bool isPlayMode() { return !m_EditorMode; }
			bool isPauseMode() { return m_PauseMode; }

			void setEditorMode() { m_EditorMode = true; }
			void setPlayMode() { m_EditorMode = false; initPlayMode(); }
			void setExitPlayMode() { m_EditorMode = true; unsetPauseMode(); exitPlayMode(); }
			void setPauseMode() { m_PauseMode = true; }
			void unsetPauseMode() { m_PauseMode = false; }

		// -------------------------------------------------------------
		// SCENE MANAGER PRIVATE METHODS
		// -------------------------------------------------------------

		private:
			void init(const std::vector<Entity>& entities, graphics::RenderPipeline& render_pipeline);

			void initPlayMode();
			void exitPlayMode();

			void updateEditorMode();
			void updatePlayMode();
			void updatePauseMode();

			void submitCamera(TransformComponent& tran, CameraComponent& cam);
			void calculateCameraTransforms(TransformComponent& tran, CameraComponent& cam);

		// -------------------------------------------------------------
		// SCENE MANAGER MEMBERS
		// -------------------------------------------------------------

		private:
			friend class SceneEvents;
			Scene* m_scene;

		public:
			bool useEditorCamera;

		private:
			bool m_EditorMode;
			bool m_PauseMode;
		}; 


} }


#endif //!MAR_ENGINE_ECS_SCENE_MANAGER_H