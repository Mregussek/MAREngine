/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */


#ifndef MAR_ENGINE_ECS_SCENE_EVENTS_H
#define MAR_ENGINE_ECS_SCENE_EVENTS_H


#include "../../mar.h"
#include "SceneManager.h"
#include "ECS/Systems.h"
#include "ECS/Entity.h"
#include "ECSLogs.h"



namespace mar {
	namespace ecs {


		class SceneEvents {
			static SceneEvents instance;

		public:

			SceneEvents() = default;

			SceneManager* scene_manager{ nullptr };

			inline static SceneEvents& Instance() { return instance; }

			void updateTransform(Entity* e, int32_t i) {
				scene_manager->initialize();

				ECS_TRACE("SCENE_EVENTS: updateTransform!");
			}

			void updateRenderables(Entity* e, int32_t i) {
				scene_manager->initialize();

				ECS_TRACE("SCENE_EVENTS: updateRenderables!");
			}

			void updatedCamera(Entity* e, int32_t i) {
				scene_manager->initialize();

				ECS_TRACE("SCENE_EVENTS: updatedCamera!");
			}

			void updatedColor(Entity* e, int32_t i) {
				scene_manager->initialize();

				ECS_TRACE("SCENE_EVENTS: updatedColor!");
			}

			void updatedTexture2D(Entity* e, int32_t i) {
				scene_manager->initialize();

				ECS_TRACE("SCENE_EVENTS: updatedTexture2D!");
			}

			void updatedCubemap(Entity* e, int32_t i) {
				scene_manager->initialize();

				ECS_TRACE("SCENE_EVENTS: updatedCubemap!");
			}

			void updatedLight(Entity* e, int32_t i) {
				scene_manager->initialize();

				ECS_TRACE("SCENE_EVENTS: updatedLight!");
			}

			void updatedScript(Entity* e, int32_t i) {
				scene_manager->initialize();

				ECS_TRACE("SCENE_EVENTS: updatedScript!");
			}

			void onEntityRemove() {
				scene_manager->initialize();

				ECS_TRACE("SCENE_EVENTS: onEntityRemove");
			}


		};


} }


#endif // !MAR_ENGINE_ECS_SCENE_EVENTS_H
