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


	/*
		The entity is a general purpose object. It only consists of a unique ID (uint32_t, which can found at entt::entity) and m_sceneRegistry. 
		Every entity is a container, where components can be attached. Entities are the base of all objects, that can be found in the scene.
	*/
	class Entity {

		friend class Scene;

		struct ChildComponent {
			std::vector<Entity> childs;
		};

	public:

		// Entity default constructor is disabled, because we don't want to store nullptr at m_registry!
		Entity() = delete;

		/*
			This is default constructor for Entity class, because we need to initialize m_sceneRegistry member! 
			If m_sceneRegistry member will stay as nullptr value, Entity instance will immedietaly crash. 
			During this constructor call entity is created.
		*/
		Entity(SceneRegistry* sceneRegistry);

		/*
			Constructor for using already created entity and its sceneRegistry instance. Used mostly in entt::registry::view lambda.
		*/
		Entity(entt::entity entt_entity, SceneRegistry* sceneRegistry);

		/*
			Default copy constructor.
		*/
		Entity(const Entity& other) = default;

		/*
			Static method for filling entity with basic components. By default it needs:
				- TagComponent (we want to have human readable tag for every entity)
				- TransformComponent (we want every entity to have its own position, rotation, scale)
				- RenderPipelineComponent (engine-only component, it remembers batches that other components are stored in, optimization)
				- ChildComponent (used to store some childs, that will be relative to base entity - parent)
		*/
		static void fillEntityWithBasicComponents(const Entity& entity);

		/*
			Method that has ability to destroy current entity. Remember to delete destroyed entity 
			array that it is stored in, it can cause a lot of damage.
		*/
		void destroyYourself() const;

		/*
			Method returns true if entity is valid (it exists and is fine).
		*/
		MAR_NO_DISCARD const bool isValid() const;

		/*
			Assigns child to current entity. Places child to array at ChildComponent.
		*/
		void assignChild(const Entity& child) const;

		/*
			Removes child from ChildComponent by its index in array.
			WARNING: child is not destroyed, only removed from array!
		*/
		void removeChild(size_t index) const;

		/*
			Removes child from ChildComponent.
			WARNING: child is not destroyed, only removed from array!
		*/
		void removeChild(const Entity& child) const;

		/*
			Returns true, if current entity contains any childs.
		*/
		MAR_NO_DISCARD bool hasChilds() const;

		/*
			Returns all childs of current entity.
		*/
		MAR_NO_DISCARD const std::vector<Entity>& getChilds() const;
		
		/*
			Returns child by its index in array.
		*/
		MAR_NO_DISCARD const Entity& getChild(size_t index) const;

		/*
			Checks, wheter entity contains some component. True, if has component.
		*/
		template<typename TComponent> MAR_NO_DISCARD const bool hasComponent() const;

		/*
			Method adds component to current entity. TComponent stands for component class, args
			are just arguments, that can be moved during creation. Make sure that component
			has constructor for given args.
			
			Returns newly created component.
		*/
		template<typename TComponent, typename... Args> TComponent& addComponent(Args&&... args) const;

		/*
			Method returns TComponent, that current entity should contain. If it has not, debugbreak is called.
		*/
		template<typename TComponent> MAR_NO_DISCARD TComponent& getComponent() const;

		/*
			Method replaces current entity's component with other entity component's values.
			Make sure that, before this method current and other entities have TComponent.
		*/
		template<typename TComponent> TComponent& replaceComponent(const Entity& other) const;

		/*
			Method replaces current entity's component with passed TComponent argument.
			Make sure that, before this method current entity has TComponent.
		*/
		template<typename TComponent> TComponent& replaceComponent(const TComponent& other) const;

		/*
			Method should be used, if you don't know whether entity has TComponent or do not. It adds it or just returns it.
		*/
		template<typename TComponent, typename... Args> MAR_NO_DISCARD TComponent& get_addComponent(Args&&... args) const;

		/*
			Method removes TComponent from current entity.
		*/
		template<typename TComponent> void removeComponent() const;

	private:
		
		SceneRegistry* m_sceneRegistry{ nullptr };
		entt::entity m_entityHandle{ entt::null };

	};


}


#include "Entity.inl"


#endif // !MAR_ENGINE_ECS_ENTITY_H