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
#include "../graphics/Renderer/RenderPipeline.h"


namespace mar::ecs {

	SceneEvents SceneEvents::s_instance;


	void SceneEvents::onTransformUpdate(const Entity* e) {
		const auto& transform = e->getComponent<TransformComponent>();
		const auto& rpc = e->getComponent<RenderPipelineComponent>();

		graphics::RenderPipeline::Instance().modifyTransform(transform, rpc.containerIndex, rpc.transformIndex);

		ECS_TRACE("SCENE_EVENTS: updateTransform!");
	}

	void SceneEvents::onRenderableAdd() {
		m_sceneManager->initialize();
	}

	void SceneEvents::onRenderableUpdate(const Entity* e) {
		m_sceneManager->initialize();

		ECS_TRACE("SCENE_EVENTS: updateRenderables!");
	}
	
	void SceneEvents::onRenderableRemove() {
		m_sceneManager->initialize();
	}

	void SceneEvents::onCameraAdd() {
		m_sceneManager->initialize();
	}

	void SceneEvents::onCameraUpdate(const Entity* e) {
		m_sceneManager->initialize();

		ECS_TRACE("SCENE_EVENTS: updatedCamera!");
	}

	void SceneEvents::onCameraRemove() {
		m_sceneManager->initialize();
	}

	void SceneEvents::onColorAdd() {
		m_sceneManager->initialize();
	}

	void SceneEvents::onColorUpdate(const Entity* e) {
		const auto& color = e->getComponent<ColorComponent>();
		const auto& rpc = e->getComponent<RenderPipelineComponent>();

		graphics::RenderPipeline::Instance().modifyColor(color, rpc.containerIndex, rpc.colorIndex);

		ECS_TRACE("SCENE_EVENTS: updatedColor!");
	}

	void SceneEvents::onColorRemove() {
		m_sceneManager->initialize();
	}

	void SceneEvents::onTexture2DAdd() {
		m_sceneManager->initialize();
	}

	void SceneEvents::onTexture2DUpdate(const Entity* e) {
		m_sceneManager->initialize();

		ECS_TRACE("SCENE_EVENTS: updatedTexture2D!");
	}

	void SceneEvents::onTexture2DRemove() {
		m_sceneManager->initialize();
	}

	void SceneEvents::onTextureCubemapAdd() {
		m_sceneManager->initialize();
	}

	void SceneEvents::onTextureCubemapUpdate(const Entity* e) {
		m_sceneManager->initialize();

		ECS_TRACE("SCENE_EVENTS: updatedCubemap!");
	}

	void SceneEvents::onTextureCubemapRemove() {
		m_sceneManager->initialize();
	}

	void SceneEvents::onLightAdd() {
		m_sceneManager->initialize();
	}

	void SceneEvents::onLightUpdate(const Entity* e) {
		const auto& transform = e->getComponent<TransformComponent>();
		const auto& light = e->getComponent<LightComponent>();
		const auto& rpc = e->getComponent<RenderPipelineComponent>();

		graphics::RenderPipeline::Instance().modifyLight(transform.center, light, rpc.containerLightIndex, rpc.lightIndex);

		ECS_TRACE("SCENE_EVENTS: updatedLight!");
	}

	void SceneEvents::onLightRemove() {
		m_sceneManager->initialize();
	}

	void SceneEvents::onScriptAdd() {
		m_sceneManager->initialize();
	}

	void SceneEvents::onScriptUpdate(const Entity* e) {
		m_sceneManager->initialize();

		ECS_TRACE("SCENE_EVENTS: updatedScript!");
	}

	void SceneEvents::onScriptRemove() {
		m_sceneManager->initialize();
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
