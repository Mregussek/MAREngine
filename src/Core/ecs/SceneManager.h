/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_ECS_SCENE_MANAGER_H
#define MAR_ENGINE_ECS_SCENE_MANAGER_H


#include "../../mar.h"
#include "SceneStorage.h"
#include "Scene.h"



namespace mar {
	namespace ecs {

		class SceneEvents;


		class SceneManager {
		// -------------------------------------------------------------
		// SCENE MANAGER PUBLIC METHODS
		// -------------------------------------------------------------

		public:
			SceneManager();

			void shutdown() { m_scene->shutdown(); }

			void initialize();
			void update();

			void setScene(Scene* scene) { m_scene = scene; }
			Scene* getScene() { return m_scene; }
			inline const SceneStorage& getStorage() const { return m_sceneStorage; }

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
			void initPlayMode();
			void exitPlayMode();

			void updateEditorMode();
			void updatePlayMode();
			void updatePauseMode();

			void submitColorEntity(const Entity& entity, TransformComponent& tran, RenderableComponent& ren, BufferStorage<maths::vec3>& storage);

			template<typename TextureType>
			void submitTextureEntity(const Entity& entity, TransformComponent& tran, RenderableComponent& ren, BufferStorage<int32_t>& storage, int32_t i);

			template<typename T>
			void submitVerticesIndices(RenderableComponent& ren, BufferStorage<T>& storage);

			template<typename T>
			void submitSampler(T& sampler, BufferStorage<T>& storage);

			void submitCamera(const Entity& entity, TransformComponent& tran);

			void calculateCameraTransforms(TransformComponent& tran, CameraComponent& cam, graphics::RenderCamera& ren_cam);

		// -------------------------------------------------------------
		// SCENE MANAGER MEMBERS
		// -------------------------------------------------------------

		private:
			friend class SceneEvents;

			SceneStorage m_sceneStorage;
			Scene* m_scene;

		public:
			bool useEditorCamera;

		private:
			bool m_EditorMode;
			bool m_PauseMode;
		}; 


} }


#endif //!MAR_ENGINE_ECS_SCENE_MANAGER_H