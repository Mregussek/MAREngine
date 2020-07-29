/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "Scene.h"
#include "Entity/Entity.h"


namespace mar {
	namespace ecs {


		Scene::Scene(std::string name)
			: m_name(name),
			updatedTransforms(false),
			updatedColors(false),
			updatedTextures2D(false),
			updatedTexturesCubemap(false),
			updatedLight(false)
		{
			m_registry = entt::registry();
		}

		Scene::~Scene() {
			m_registry.clear();
		}

		Entity& Scene::createEntity() {
			Entity entity{ this };

			entity.addDefault();
			entity.addComponent<TagComponent>(ECS_TAG);

			entities.push_back(entity);

			return entities[entities.size() - 1];
		}

		void Scene::setName(std::string name) {
			m_name = name;
		}


} }