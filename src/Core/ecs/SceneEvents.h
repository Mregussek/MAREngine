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
		public:

			static SceneManager* scene_manager;

			static void updateTransform(Entity* e, int32_t i) {
				auto& tran = e->getComponent<TransformComponent>();
				System::handleTransformComponent(tran);

				scene_manager->initialize();

				ECS_TRACE("SCENE_EVENTS: updateTransform!");
			}

			static void updateRenderables(Entity* e, int32_t i) {
				scene_manager->initialize();

				ECS_TRACE("SCENE_EVENTS: updateRenderables!");
			}

			static void updatedCamera(Entity* e, int32_t i) {
				scene_manager->initialize();

				ECS_TRACE("SCENE_EVENTS: updatedCamera!");
			}

			static void updatedColor(Entity* e, int32_t i) {
				scene_manager->initialize();

				ECS_TRACE("SCENE_EVENTS: updatedColor!");
			}

			static void updatedTexture2D(Entity* e, int32_t i) {
				scene_manager->initialize();

				ECS_TRACE("SCENE_EVENTS: updatedTexture2D!");
			}

			static void updatedCubemap(Entity* e, int32_t i) {
				scene_manager->initialize();

				ECS_TRACE("SCENE_EVENTS: updatedCubemap!");
			}

			static void updatedLight(Entity* e, int32_t i) {
				scene_manager->initialize();

				ECS_TRACE("SCENE_EVENTS: updatedLight!");
			}


		};


} }


#endif // !MAR_ENGINE_ECS_SCENE_EVENTS_H
