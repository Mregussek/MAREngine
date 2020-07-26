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
			clear();
		}

		Entity Scene::createEntity() {
			Entity entity{ m_registry.create(), this };

			entity.addComponent<TagComponent>();
			entity.addComponent<RenderableComponent>();
			entity.addComponent<TransformComponent>();

			return entity;
		}

		void Scene::update() {

		}

		void Scene::clear() {
			m_registry.clear();
		}


} }