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


namespace mar::ecs {

	SceneEvents SceneEvents::s_instance;


	void SceneEvents::updateTransform(const Entity* e) {
		m_sceneManager->initialize();

		ECS_TRACE("SCENE_EVENTS: updateTransform!");
	}

	void SceneEvents::updateRenderables(const Entity* e) {
		m_sceneManager->initialize();

		ECS_TRACE("SCENE_EVENTS: updateRenderables!");
	}

	void SceneEvents::updatedCamera(const Entity* e) {
		m_sceneManager->initialize();

		ECS_TRACE("SCENE_EVENTS: updatedCamera!");
	}

	void SceneEvents::updatedColor(const Entity* e) {
		m_sceneManager->initialize();

		ECS_TRACE("SCENE_EVENTS: updatedColor!");
	}

	void SceneEvents::updatedTexture2D(const Entity* e) {
		m_sceneManager->initialize();

		ECS_TRACE("SCENE_EVENTS: updatedTexture2D!");
	}

	void SceneEvents::updatedCubemap(const Entity* e) {
		m_sceneManager->initialize();

		ECS_TRACE("SCENE_EVENTS: updatedCubemap!");
	}

	void SceneEvents::updatedLight(const Entity* e) {
		m_sceneManager->initialize();

		ECS_TRACE("SCENE_EVENTS: updatedLight!");
	}

	void SceneEvents::updatedScript(const Entity* e) {
		m_sceneManager->initialize();

		ECS_TRACE("SCENE_EVENTS: updatedScript!");
	}

	void SceneEvents::onEntityCopy() {
		m_sceneManager->initialize();

		ECS_TRACE("SCENE_EVENTS: onEntityCopy");
	}

	void SceneEvents::onEntityRemove() {
		m_sceneManager->initialize();

		ECS_TRACE("SCENE_EVENTS: onEntityRemove");
	}

	void SceneEvents::onCollectionTransformUpdate() {
		m_sceneManager->initialize();

		ECS_TRACE("SCENE_EVENTS: onCollectionTransformUpdate");
	}

	void SceneEvents::onCollectionRemove() {
		m_sceneManager->initialize();

		ECS_TRACE("SCENE_EVENTS: onCollectionRemove");
	}

	void SceneEvents::onCollectionOBJloaded(const EntityCollection& collection) {
		m_sceneManager->initialize();

		ECS_TRACE("SCENE_EVENTS: onCollectionOBJloaded");
	}


}
