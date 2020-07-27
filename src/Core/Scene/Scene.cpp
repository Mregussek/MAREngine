/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "Scene.h"
#include "Entity/Entity.h"


namespace mar {
	namespace ecs {


		Scene::Scene() {

		}

		Scene::~Scene() {
			m_registry.clear();
		}

		Entity Scene::createEntity() {
			Entity entity{ m_registry.create(), this };

			entity.addComponent<TagComponent>();
			entity.addComponent<RenderableComponent>();
			entity.addComponent<TransformComponent>();

			m_entities.emplace_back(entity);

			return entity;
		}

		void Scene::update() {

		}


} }