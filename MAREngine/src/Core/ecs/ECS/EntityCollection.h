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


#ifndef MAR_ENGINE_ECS_ENTITY_COLLECTION_H
#define MAR_ENGINE_ECS_ENTITY_COLLECTION_H


#include "../../../mar.h"
#include "../ECSLogs.h"
#include "../Scene.h"
#include "Entity.h"
#include "Components.h"


namespace mar {
	namespace ecs {

		class Entity;
		class Scene;


		class EntityCollection {
			friend class Scene;

			std::vector<Entity> m_entities;
			entt::entity m_entityHandle{ entt::null };
			Scene* m_scene{ nullptr };

		public:
			EntityCollection() = default;
			EntityCollection(Scene* scene)
				: m_scene(scene),
				m_entityHandle(scene->m_registry.create())
			{}

			EntityCollection(const EntityCollection& other)
				: m_scene(other.m_scene),
				m_entityHandle(other.m_entityHandle),
				m_entities(other.m_entities)
			{}

			void destroyYourself() {
				for (size_t i = 0; i < m_entities.size(); i++) {
					if (m_entities[i].isValid()) {
						m_entities[i].destroyYourself();
						m_entities.erase(m_entities.begin() + i);

						ECS_TRACE("ENTITY_COLLECTION: from collection entity at {} is deleted!", index, i);
					}
					else {
						ECS_TRACE("ENTITY_COLLECTION: from collection entity at {} is not valid, so it is not deleted!!", index, i);
					}
				}
			}

			Entity& createEntity() {
				ECS_INFO("ENTITY_COLLECTION: going to create entity!");

				Entity entity{ m_scene };

				entity.addDefault();
				entity.addComponent<TagComponent>(ECS_TAG);
				entity.addComponent<TransformComponent>(ECS_TRANSFORM);

				m_entities.push_back(entity);

				ECS_INFO("EntityCollection: created entity!");

				return m_entities[m_entities.size() - 1];
			}

			void destroyEntity(int32_t entity_index) {
				if (m_entities[entity_index].isValid()) {
					m_entities[entity_index].destroyYourself();
					m_entities.erase(m_entities.begin() + entity_index);

					ECS_INFO("ENTITY_COLLECTION: destroyed entity {} at collection!", entity_index);
				}
				else {
					ECS_INFO("ENTITY_COLLECTION: entity {} at collection is not valid, so it cannot be destroyed!", entity_index);
				}
			}

			template<typename T, typename... Args>
			T& addComponent(Args&&... args) {
				MAR_CORE_ASSERT(!m_scene->m_registry.has<T>(m_entityHandle), "ENTITY_COLLECTION: already has CollectionTagComponent!");

				return m_scene->m_registry.emplace<T>(m_entityHandle, std::forward<Args>(args)...);
			}

			template<typename T>
			T& getComponent() const {
				MAR_CORE_ASSERT(m_scene->m_registry.has<T>(m_entityHandle), "ENTITY_COLLECTION: does not have this component!");

				return m_scene->m_registry.get<T>(m_entityHandle);
			}

			const std::vector<Entity>& getEntities() const { return m_entities; }
			Entity& getEntity(size_t index) { return m_entities[index]; }
		};


} }


#endif // !MAR_ENGINE_ECS_ENTITY_COLLECTION_H
