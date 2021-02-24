/***********************************************************************
* @internal @copyright
*
*  				MAREngine - open source 3D game engine
*
* Copyright (C) 2020-present Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
* All rights reserved.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
************************************************************************/


#ifndef MAR_ENGINE_ENTITY_H
#define MAR_ENGINE_ENTITY_H


#include "../../../mar.h"
#include "EntityDefinitions.h"
#include "../ECSLogs.h"
#include "../Components/Components.h"


namespace marengine {


	/**
	 * @class Entity Entity.h "Core/ecs/Entity/Entity.h" 
	 * @brief The entity is a general purpose object.
	 * It only consists of a unique ID (uint32_t, which can found at entt::entity) and m_sceneRegistry.
	 * Every entity is a container, where components can be attached. Entities are the base of all objects, that can be found in the scene.
	 */
	class Entity {

		friend class Scene;

		struct ChildComponent {
			FEntityArray childs;
		};

	public:

		/// @brief Entity default constructor is disabled, because we don't want to store nullptr at m_registry!
		Entity() = delete;

		/**
		 * @brief This is default constructor for Entity class, because we need to initialize m_sceneRegistry member!
		 * If m_sceneRegistry member will stay as nullptr value, Entity instance will immedietaly crash.
		 * During this constructor call entity is created.
		 * @warning Constructor does not check, if pSceneRegistry is valid. It creates Entity in place, ensure that
		 * given pointer is correct.
		 * @param pSceneRegistry valid entt::registry pointer to which entity will belong to.
		 */
		Entity(entt::registry* pSceneRegistry);

		/**
		 * @brief Constructor for using already created entity and its sceneRegistry instance.
		 * Used mostly in entt::registry::view lambda.
		 * @warning Constructor does not check, if pSceneRegistry is valid. It creates Entity in place, ensure that
		 * given pointer is correct.
		 * @param entt_entity valid entt::entity, which can be reassigned to new entity object
		 * @param sceneRegistry valid entt::registry, to which entt_entity belongs to.
		 */
		Entity(entt::entity enttEntity, entt::registry* pSceneRegistry);

		/// @brief Default copy constructor.
		Entity(const Entity& other) = default;

		/**
		 * @brief Static method for filling entity with basic components. By default it needs:
		 * - TagComponent (we want to have human readable tag for every entity)
		 * - TransformComponent (we want every entity to have its own position, rotation, scale)
		 * - LightBatchInfoComponent (engine-only component, it remembers light batches that other components are stored in, optimization)
		 * - MeshBatchInfoComponent (engine-only component, it remembers mesh batches that other components are stored in, optimization)
		 * - ChildComponent (used to store some childs, that will be relative to base entity - parent)
		 * @param entity entity, which will be filled with basic components
		 */
		static void fillEntityWithBasicComponents(const Entity& entity);

		/**
		 * @brief Method that has ability to destroy current entity. Remember to delete destroyed entity
		 * array that it is stored in, it can cause a lot of damage.
		 */
		void destroyYourself() const;

		/**
		 * @brief Method checks, if entity is valid one and returns result.
		 * @return returns true if entity is valid (it exists and is fine).
		 */
		MAR_NO_DISCARD const bool isValid() const;

		/**
		 * @brief Assigns child to current entity. Places child to array at ChildComponent.
		 * @warning Make sure that child is a valid entity!
		 * @param child valid entity, which will be assigned as child
		 */
		void assignChild(const Entity& child) const;

		/**
		 * @brief Removes child from ChildComponent by its index in array.
		 * @warning child is not destroyed, only removed from array!
		 * @param index index at which child will be removed
		 */
		void removeChild(size_t index) const;

		/**
		 * @brief Removes child from ChildComponent, only if child is assigned to entity.
		 * @warning child is not destroyed, only removed from array!
		 * @param child child, which we want to be removed from array
		 */
		void removeChild(const Entity& child) const;

		/**
		 * @brief Method checks, if current entity contains childs and returns result.
		 * @return Returns true, if current entity contains any childs.
		 */
		MAR_NO_DISCARD bool hasChilds() const;

		/**
		 * @brief Returns childs assigned to an entity in array.
		 * @return Returns all childs of current entity.
		 */
		MAR_NO_DISCARD const FEntityArray& getChilds() const;
		
		/**
		 * @brief Returns child by its index in array.
		 * @warning Method does not check if it is valid entity! If index is too large, it may cause overflow.
		 * @param index index of child
		 * @return child instance at given index
		 */
		MAR_NO_DISCARD const Entity& getChild(size_t index) const;

		/**
		 * @brief Checks, if current entity has TComponent assigned and returns result.
		 * @tparam TComponent structure type of component
		 * @return returns true, if entity contains component
		 */
		template<typename TComponent> MAR_NO_DISCARD const bool hasComponent() const;

		/**
		 * @brief Method emplaces TComponent object at current entity instance and returns reference
		 * to newly created object. If needed you can pass arguments, but then please make sure that
		 * TComponent contains constructor that takes those arguments.
		 * @tparam TComponent structure type of component
		 * @tparam Args variadic template, which gives ability to call specific constructor during creation
		 * @param args variadic parameter, pass all args that TComponent needs during creation
		 * @return Returns newly created component.
		 */
		template<typename TComponent, typename... Args> TComponent& addComponent(Args&&... args) const;

		/**
		 * @brief Method returns TComponent, that current entity should contain.
		 * @warning If current entity does not contain TComponent debugbreak is called.
		 * @tparam TComponent structure type of component
		 * @return TComponent's instance assigned to current entity
		 */
		template<typename TComponent> MAR_NO_DISCARD TComponent& getComponent() const;

		/**
		 * @brief Method replaces current entity's component with other entity component's values.
		 * Make sure that, before this method current and other entities have TComponent.
		 * @tparam TComponent structure type of component
		 * @param other entity, from which we want component to be copied to current entity
		 * @return Returns newly created object at current entity (copied from given entity) 
		 */
		template<typename TComponent> TComponent& replaceComponent(const Entity& other) const;

		/**
		 * @brief Method replaces current entity's component with passed TComponent argument.
		 * Make sure that, before this method current entity has TComponent.
		 * @tparam TComponent structure type of component
		 * @param other component, which will be copied to current entity
		 * @return Returns newly created object at current entity (copied given component) 
		 */
		template<typename TComponent> TComponent& replaceComponent(const TComponent& other) const;

		/**
		 * @brief Method should be used, if you don't know whether entity has TComponent or do not. It adds it or just returns it.
		 * Remember that, you should know, when entity can contain TComponent.
		 * @tparam TComponent structure type of component
		 * @tparam Args variadic template, which gives ability to call specific constructor during creation
		 * @param args variadic parameter, pass all args that TComponent needs during creation
		 * @return Returns newly created component or already existing
		 */
		template<typename TComponent, typename... Args> MAR_NO_DISCARD TComponent& get_addComponent(Args&&... args) const;

		/**
		 * @brief Method removes TComponent from current entity.
		 * @warning Method does not check if entity contains component, it just removes it!
		 * @tparam TComponent structure type of component
		 */
		template<typename TComponent> void removeComponent() const;

	private:
		
		entt::registry* m_pSceneRegistry{ nullptr };
		entt::entity m_entityHandle{ entt::null };

	};


}


#include "Entity.inl"


#endif // !MAR_ENGINE_ENTITY_H