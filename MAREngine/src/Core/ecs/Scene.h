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


#ifndef MAR_ENGINE_SCENE_H
#define MAR_ENGINE_SCENE_H


#include "../../mar.h"
#include "Components/Components.h"
#include "Entity/EntityDefinitions.h"
#include "Entity/Entity.h"


namespace marengine {


	/*
		Scene has information about all important entities, has abilities to create and destroy entities,
		contains main class that rule them all - SceneRegistry. You can consider Scene as the game itself.
		If you want to create a new game, just create some entities, attach components to them and push them
		to rendering engine.
	*/
	class Scene {
	public:

		// Default Constructor deleted, because we want to create registry explicitly
		Scene() = delete;
		
		// Default Constructor, with which we assign sceneName and create scene registry
		Scene(std::string name);

		/*
			Creates empty scene with 2 entities:
				- cameraEntity (main camera assigned)
				- lightEntity (pointLight assigned)

			Scene is created on the heap using 'new', make sure to delete it when the job is done!
		*/
		MAR_NO_DISCARD static Scene* createEmptyScene(std::string name);

		// Method is responsible for whole cleanup. It destroys all entities and registry itself.
		void close();

		// Method creates Entity at m_entities, assigns to it some basic components and returns it.
		MAR_NO_DISCARD const Entity& createEntity();

		// Method checks if given entity exists in m_entities, if so entity is being destroyed and popped from m_entities.
		void destroyEntity(const Entity& entity);

		// Method returns all entities.
		MAR_NO_DISCARD const FEntityArray& getEntities() const;

		// Sets new scene name
		void setName(std::string newSceneName);

		// Returns scene name
		MAR_NO_DISCARD const std::string& getName() const;

		// Sets scene background color value
		void setBackground(maths::vec3 newSceneBackgroundColor);

		// Returns scene background color value. Non-const method, because we want to modify it in widget panels
		MAR_NO_DISCARD maths::vec3& getBackground();

		// Returns scene registry. Non-const, because it will be used by entities. Please, use this carefully.
		MAR_NO_DISCARD entt::registry* getRegistry();
		
		// Returns view at all entities that contains TComponent. View can be iterated with lambda [](entt::entity entt_entity, TComponent& component) {};
		template<typename TComponent> MAR_NO_DISCARD auto getView();

		// Returns component from entt::entity. Used only in lambda methods at view.
		template<typename TComponent> MAR_NO_DISCARD TComponent& getComponent(entt::entity entt_entity);

	private:

		std::string m_name{ "Empty Scene" };
		FEntityArray m_entities;

		maths::vec3 m_backgroundColor{ 0.22f, 0.69f, 0.87f };
		//SceneRegistry m_sceneRegistry;
		entt::registry m_sceneRegistry;

	};


}


#include "Scene.inl"


#endif // !MAR_ENGINE_ECS_SCENE_H