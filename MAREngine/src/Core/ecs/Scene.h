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


#ifndef MAR_ENGINE_SCENE_H
#define MAR_ENGINE_SCENE_H


#include "../../mar.h"
#include "Components/Components.h"
#include "Entity/EntityDefinitions.h"
#include "Entity/Entity.h"


namespace marengine {


	/**
	* @class Scene Scene.h "Core/ecs/Scene.h"
	* @brief Scene has information about all important entities, has abilities to create and destroy entities
	* Scene contains main class that rule them all - SceneRegistry. You can consider Scene as the game itself.
	* If you want to create a new game, just create some entities, attach components to them and push them
	* to rendering engine.
	*/
	class Scene {
	public:

		/// @brief Default Constructor deleted, because we want to create registry explicitly
		Scene() = delete;
		
		/// @brief Default Constructor, with which we assign sceneName and create scene registry
		Scene(std::string name);
		
		/**
		* @brief Creates empty scene with cameraEntity and lightEntity.
		* Two created entities by default:
		* - cameraEntity (main camera assigned)
		* - lightEntity (pointLight assigned)
		* Scene is created on the heap using 'new', make sure to delete it when the job is done!
		* @param sceneName name of the new created scene
		* @return scene created scene with "new" operator
		*/
		MAR_NO_DISCARD static Scene* createEmptyScene(std::string sceneName);

		/// @brief Method is responsible for whole cleanup. It destroys all entities and registry itself.
		void close();

		/**
		* @brief Method creates Entity at m_entities, assigns to it some basic components and returns it.
		* @return entity created entity pushed to scene
		*/
		MAR_NO_DISCARD const Entity& createEntity();

		/**
		* @brief Method checks if given entity exists in m_entities, if so entity is being destroyed and popped from m_entities.
		* @param entity entity that will be deleted from current scene
		*/
		void destroyEntity(const Entity& entity);

		/**
		* @brief Method returns all entities.
		* @return m_entities const reference to array of entities
		*/
		MAR_NO_DISCARD const FEntityArray& getEntities() const;

		/**
		* @brief Sets new scene name
		* @param newSceneName new name for current scene
		*/
		void setName(std::string newSceneName);

		/**
		* @brief Returns current scene name
		* @return  m_name current name scene
		*/
		MAR_NO_DISCARD const std::string& getName() const;

		/**
		* @brief Sets scene background color value
		* @param newSceneBackgroundColor new vec3 background color for current scene
		*/
		void setBackground(maths::vec3 newSceneBackgroundColor);

		/**
		* @brief Returns scene background color value.
		* Non-const method, because we want to modify it during editor mode / game mode
		* @return m_backgroundColor current background color
		*/
		MAR_NO_DISCARD maths::vec3& getBackground();

		/**
		* @brief Returns pointer to scene registry.
		* Non-const, because it will be used by entities. Please, use this carefully.
		* @return m_sceneRegistry pointer to registry of the scene
		*/
		MAR_NO_DISCARD entt::registry* getRegistry();
		
		/**
		* @brief Returns view at all entities that contains TComponent. 
		* View can be iterated with lambda [](entt::entity entt_entity, TComponent& component) {};
		* @return entt::basic_view view for given TComponent
		*/
		template<typename TComponent> MAR_NO_DISCARD auto getView();

		/**
		* @brief Returns component from entt::entity. Used only in lambda methods at view.
		* @param entt_entity entity's unique index
		* @return TComponent_instance reference to instance of entt_entity's TComponent
		*/
		template<typename TComponent> MAR_NO_DISCARD TComponent& getComponent(entt::entity entt_entity);

	private:

		std::string m_name{ "Empty Scene" };
		FEntityArray m_entities;

		maths::vec3 m_backgroundColor{ 0.22f, 0.69f, 0.87f };
		entt::registry m_sceneRegistry;

	};


}


#include "Scene.inl"


#endif // !MAR_ENGINE_ECS_SCENE_H
