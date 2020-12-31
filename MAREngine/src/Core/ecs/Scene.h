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


#ifndef MAR_ENGINE_ECS_SCENE_H
#define MAR_ENGINE_ECS_SCENE_H


#include "../../mar.h"
#include "Entity/EntityContainer.h"


namespace marengine {


	class Scene {
		
		friend class Entity;
		friend class EntityCollection;

	public:

		Scene() = default;
		Scene(std::string name);

		MAR_NO_DISCARD static Scene* createEmptyScene(std::string name);
		void shutdown();

		MAR_NO_DISCARD const Entity& createEntity();
		void destroyEntity(const Entity& entity);

		MAR_NO_DISCARD const EntityCollection& createCollection();
		void destroyCollection(const EntityCollection& collection);

		// --- SET METHODS --- //

		void setName(std::string name) { m_name = std::move(name); }
		void setBackground(maths::vec3 v) { m_backgroundColor = std::move(v); }
		
		// --- GET METHODS --- //

		MAR_NO_DISCARD const std::string& getName() const { return m_name; }
		MAR_NO_DISCARD maths::vec3& getBackground() { return m_backgroundColor; }
		MAR_NO_DISCARD SceneRegistry* getRegistry() { return &m_sceneRegistry; }
		MAR_NO_DISCARD const std::vector<Entity>& getEntities() const;
		MAR_NO_DISCARD const std::vector<EntityCollection>& getCollections() const;

		template<typename T> 
		MAR_NO_DISCARD auto getView() {
			return m_sceneRegistry.m_registry.view<T>();
		}

		template<typename T> 
		MAR_NO_DISCARD T& getComponent(entt::entity entity) {
			MAR_CORE_ASSERT(m_sceneRegistry.m_registry.has<T>(entity), "Passed entity does not have component");
			return m_sceneRegistry.m_registry.get<T>(entity);
		}

	private:

		std::string m_name{ "Empty Scene" };
		EntityContainer m_container;

		maths::vec3 m_backgroundColor{ 0.22f, 0.69f, 0.87f };
		SceneRegistry m_sceneRegistry;

	};


}


#endif // !MAR_ENGINE_ECS_SCENE_H