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


#include "Scene.h"
#include "Components/Components.h"
#include "Entity/Entity.h"
#include "ECSLogs.h"


namespace marengine {


	Scene::Scene(std::string name)
		: m_name(std::move(name))
	{
		m_sceneRegistry = SceneRegistry();

		ECS_INFO("SCENE: scene {} is created, with entt::registry! (called constructor)", m_name);
	}

	void Scene::shutdown() {
		ECS_TRACE("SCENE: closing scene {}...", m_name);

		std::for_each(m_container.m_entities.begin(), m_container.m_entities.end(), [this](const Entity& entity) {
			destroyEntity(entity);
		});

		m_sceneRegistry.cleanup();

		ECS_INFO("SCENE: registry is cleared! (called destructor)");
	}

	Scene* Scene::createEmptyScene(std::string name) {
		Scene* scene = new Scene(std::move(name));

		{ // create Camera entity
			const Entity& cameraEntity{ scene->createEntity() };
			cameraEntity.addComponent<CameraComponent>();

			TagComponent& tag = cameraEntity.getComponent<TagComponent>();
			tag.tag = "CameraEntity";
		}
		{ // create Light Entity
			const Entity& lightEntity{ scene->createEntity() };
			lightEntity.addComponent<LightComponent>();

			TagComponent& tag = lightEntity.getComponent<TagComponent>();
			tag.tag = "LightEntity";
		}

		return scene;
	}

	// -------------------------------------------------------------
	// ENTITIES MANAGEMENT
	// -------------------------------------------------------------

	const Entity& Scene::createEntity() {
		ECS_TRACE("SCENE: creating entity at scene {}...", m_name);

		const Entity& entity{ m_container.m_entities.emplace_back(&m_sceneRegistry) };
		Entity::fillEntityWithBasicComponents(entity);

		ECS_DEBUG("SCENE: created entity {} at scene {}, current entities size = {}!", entity.m_entityHandle, m_name, m_container.m_entities.size());

		return entity;
	}

	void Scene::destroyEntity(const Entity& entity) {
		ECS_TRACE("SCENE: trying to destroy entity {} at scene {}...", entity.m_entityHandle, m_name);

		auto it = std::find_if(m_container.m_entities.begin(), m_container.m_entities.end(), [&entity](const Entity& iterator) {
			return 	iterator.m_entityHandle == entity.m_entityHandle && iterator.m_sceneRegistry == entity.m_sceneRegistry && entity.isValid();
		});

		const bool canDestroyEntity{ it != m_container.m_entities.end() };

		if (canDestroyEntity) {
			ECS_TRACE("SCENE: destroying entity {} at scene {}...", entity.m_entityHandle, m_name);

			(*it).destroyYourself();
			m_container.m_entities.erase(it);
		}
		else {
			ECS_WARN("SCENE: cannot find destroyable entity {} at scene {}..., won't be destroyed!", entity.m_entityHandle, m_name);
		}
	}

	const std::vector<Entity>& Scene::getEntities() const { 
		return m_container.m_entities;
	}


}
