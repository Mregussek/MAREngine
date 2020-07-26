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

		}

		Entity Scene::createEntity() {
			Entity entity{ m_registry.create(), this };
			entity.addComponent<IdentifierComponent>();
			entity.addComponent<TransformComponent>();



			return entity;
		}

		void Scene::update() {
			m_vertices.clear();
			m_indices.clear();
			m_samplers.clear();
			m_transforms.clear();
			m_colors.clear();

			for (auto& entity : m_entities) {
				auto& tran = entity.getComponent<TransformComponent>();
				m_transforms.push_back(tran);

				auto& col = entity.getComponent<ColorComponent>();
				m_colors.push_back(col);
				m_samplers.push_back(0);

				auto& ren = entity.getComponent<RenderableComponent>();
				m_vertices.insert(m_vertices.end(), ren.vertices.begin(), ren.vertices.end());
				m_indices.insert(m_indices.end(), ren.indices.begin(), ren.indices.end());
			}
		}


} }