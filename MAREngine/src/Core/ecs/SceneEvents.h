/**
 *           MAREngine - open source 3D game engine
 * Copyright (C) 2020-present Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 * All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
**/


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

			void onCollectionRemove() {
				scene_manager->initialize();

				ECS_TRACE("SCENE_EVENTS: onCollectionRemove");
			}

			void onCollectionOBJloaded(EntityCollection& collection) {
				scene_manager->initialize();

				ECS_TRACE("SCENE_EVENTS: onCollectionOBJloaded");
			}


		};


} }


#endif // !MAR_ENGINE_ECS_SCENE_EVENTS_H
