/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "Scene.h"
#include "Entity/Entity.h"


namespace mar {
	namespace ecs {


		Scene::Scene(const char* name)
			: m_name(name),
			updatedTransforms(false),
			updatedColors(false),
			updatedTextures2D(false),
			updatedTexturesCubemap(false),
			updatedLight(false)
		{}

		Scene::~Scene() {
			m_registry.clear();
		}

		Entity Scene::createEntity() {
			Entity entity{ m_registry.create(), this };

			entity.addComponent<TagComponent>(ECS_TAG);

			entities.push_back(entity);

			return entity;
		}

		void Scene::setName(const char* name) {
			m_name = name;
		}


} }