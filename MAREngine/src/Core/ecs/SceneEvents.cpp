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


#include "SceneEvents.h"
#include "SceneManager.h"
#include "Entity/Entity.h"
#include "Entity/EntityCollection.h"
#include "ECSLogs.h"


namespace mar {
	namespace ecs {

		SceneEvents SceneEvents::instance;


		void SceneEvents::updateTransform(Entity* e, int32_t i) {
			scene_manager->initialize();

			ECS_TRACE("SCENE_EVENTS: updateTransform!");
		}

		void SceneEvents::updateRenderables(Entity* e, int32_t i) {
			scene_manager->initialize();

			ECS_TRACE("SCENE_EVENTS: updateRenderables!");
		}

		void SceneEvents::updatedCamera(Entity* e, int32_t i) {
			scene_manager->initialize();

			ECS_TRACE("SCENE_EVENTS: updatedCamera!");
		}

		void SceneEvents::updatedColor(Entity* e, int32_t i) {
			scene_manager->initialize();

			ECS_TRACE("SCENE_EVENTS: updatedColor!");
		}

		void SceneEvents::updatedTexture2D(Entity* e, int32_t i) {
			scene_manager->initialize();

			ECS_TRACE("SCENE_EVENTS: updatedTexture2D!");
		}

		void SceneEvents::updatedCubemap(Entity* e, int32_t i) {
			scene_manager->initialize();

			ECS_TRACE("SCENE_EVENTS: updatedCubemap!");
		}

		void SceneEvents::updatedLight(Entity* e, int32_t i) {
			scene_manager->initialize();

			ECS_TRACE("SCENE_EVENTS: updatedLight!");
		}

		void SceneEvents::updatedScript(Entity* e, int32_t i) {
			scene_manager->initialize();

			ECS_TRACE("SCENE_EVENTS: updatedScript!");
		}

		void SceneEvents::onEntityCopy() {
			scene_manager->initialize();

			ECS_TRACE("SCENE_EVENTS: onEntityCopy");
		}

		void SceneEvents::onEntityRemove() {
			scene_manager->initialize();

			ECS_TRACE("SCENE_EVENTS: onEntityRemove");
		}

		void SceneEvents::onCollectionTransformUpdate() {
			scene_manager->initialize();

			ECS_TRACE("SCENE_EVENTS: onCollectionTransformUpdate");
		}

		void SceneEvents::onCollectionRemove() {
			scene_manager->initialize();

			ECS_TRACE("SCENE_EVENTS: onCollectionRemove");
		}

		void SceneEvents::onCollectionOBJloaded(EntityCollection& collection) {
			scene_manager->initialize();

			ECS_TRACE("SCENE_EVENTS: onCollectionOBJloaded");
		}


} }
