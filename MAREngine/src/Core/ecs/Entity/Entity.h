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


#ifndef MAR_ENGINE_ECS_ENTITY_H
#define MAR_ENGINE_ECS_ENTITY_H


#include "../../../mar.h"
#include "../ECSLogs.h"
#include "../SceneRegistry.h"
#include "../Components/Components.h"


namespace marengine {


	class Entity {

		friend class Scene;

		struct ChildComponent {
			std::vector<Entity> childs;
		};

	public:

		// Entity default constructor is disabled, because we don't want to store nullptr at m_registry!
		Entity() = delete;
		Entity(SceneRegistry* sceneRegistry);
		Entity(entt::entity entt_entity, SceneRegistry* sceneRegistry);
		Entity(const Entity& other) = default;

		static void fillEntityWithBasicComponents(const Entity& entity);

		void destroyYourself() const;

		MAR_NO_DISCARD const bool isValid() const;

		void assignChild(const Entity& entity) const;
		void removeChild(size_t index) const;
		void removeChild(const Entity& entity) const;

		bool hasChilds() const;
		const std::vector<Entity>& getChilds() const;
		const Entity& getChild(size_t index) const;

		template<typename T> 
		MAR_NO_DISCARD const bool hasComponent() const;

		template<typename T, typename... Args> 
		T& addComponent(Args&&... args) const;

		template<typename T>
		MAR_NO_DISCARD T& getComponent() const;

		template<typename T>
		T& replaceComponent(const Entity& other) const;

		template<typename T>
		T& replaceComponent(const T& other) const;

		template<typename T, typename... Args>
		MAR_NO_DISCARD T& get_addComponent(Args&&... args) const;

		template<typename T>
		void removeComponent() const;

	private:
		
		entt::entity m_entityHandle{ entt::null };
		SceneRegistry* m_sceneRegistry{ nullptr };

	};


}


#include "Entity.inl"


#endif // !MAR_ENGINE_ECS_ENTITY_H