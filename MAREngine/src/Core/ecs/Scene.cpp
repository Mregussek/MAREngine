/***********************************************************************
* @internal @copyright
*
*  				MAREngine - open source 3D game engine
*
* Copyright (C) 2020-present Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
* All rights reserved.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
************************************************************************/


#include "Scene.h"
#include "ECSLogs.h"


namespace marengine {


	Scene::Scene(std::string name) :
		m_name(std::move(name)),
		m_sceneRegistry(entt::registry())
	{}

	Scene* Scene::createEmptyScene(std::string sceneName) {
		Scene* pScene{ new Scene(std::move(sceneName)) };

		{ // create Camera entity
			const Entity& cameraEntity{ pScene->createEntity() };
			CameraComponent& cameraComponent{ cameraEntity.addComponent<CameraComponent>() };
			cameraComponent.id = "main";

			TagComponent& tag{ cameraEntity.getComponent<TagComponent>() };
			tag.tag = "CameraEntity";
		}
		{ // create Light Entity
			const Entity& pointLightEntity{ pScene->createEntity() };
			pointLightEntity.addComponent<PointLightComponent>();

			TagComponent& tag{ pointLightEntity.getComponent<TagComponent>() };
			tag.tag = "LightEntity";
		}

		return pScene;
	}

	void Scene::close() {
		for (const auto& entity : m_entities) {
			destroyEntity(entity);
		}

		m_entities.clear();
		m_sceneRegistry.clear();

		ECS_INFO("SCENE: registry is cleared! (called destructor)");
	}

	const Entity& Scene::createEntity() {
		ECS_INFO("SCENE: going to create entity!");

		const Entity& entity{ m_entities.emplace_back(&m_sceneRegistry) };
		Entity::fillEntityWithBasicComponents(entity);

		ECS_INFO("SCENE: created entity {} at sceme {}!", entity.m_entityHandle, m_name);

		return entity;
	}

	void Scene::destroyEntity(const Entity& entity) {
		ECS_INFO("SCENE: going to destroy entity at {}!", entity.m_entityHandle);

		auto it = std::find_if(m_entities.begin(), m_entities.end(), [&entity](const Entity& iterator) {
			return 	&iterator == &entity;
		});

		const bool canDestroyEntity{ it != m_entities.end() && (*it).isValid() };

		if (canDestroyEntity) {
			it->destroyYourself();
			m_entities.erase(it);
		}
	}

	void Scene::setName(std::string newSceneName) {
		m_name = std::move(newSceneName);
	}

	void Scene::setBackground(maths::vec3 newSceneBackgroundColor) {
		m_backgroundColor = newSceneBackgroundColor;
	}

	MAR_NO_DISCARD const std::string& Scene::getName() const { 
		return m_name; 
	}

	MAR_NO_DISCARD maths::vec3& Scene::getBackground() { 
		return m_backgroundColor;
	}

	MAR_NO_DISCARD entt::registry* Scene::getRegistry() { 
		return &m_sceneRegistry; 
	}

	const FEntityArray& Scene::getEntities() const { 
		return m_entities;
	}

	const bool Scene::isValid(entt::entity enttEntity) const {
		return m_sceneRegistry.valid(enttEntity);
	}


}
