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
#include "../graphics/Renderer/RenderCamera.h"
#include "Entity/EntityContainer.h"


namespace mar {
	namespace ecs {


		class Scene {
			// ----------------------------------------------------
			// SCENE PUBLIC METHODS
			// ----------------------------------------------------

		public:
			Scene(std::string name);

			void shutdown();

			static Scene* createEmptyScene(std::string name);

			Entity& createEntity();
			void destroyEntity(int32_t index);

			EntityCollection& createCollection();
			void destroyCollection(int32_t index);
			void destroyEntityAtCollection(int32_t collection_index, int32_t entity_index);

			// --- SET METHODS --- //

			void setName(std::string name) { m_name = std::move(name); }
			void setBackground(maths::vec3 v) { m_backgroundColor = std::move(v); }
			
			// --- GET METHODS --- //

			inline const std::string& getName() const { return m_name; }
			inline maths::vec3& getBackground() { return m_backgroundColor; }

			const std::vector<Entity>& getEntities() const;
			std::vector<Entity>& getEntities();
			Entity& getEntity(size_t index);

			const std::vector<EntityCollection>& getCollections() const;
			std::vector<EntityCollection>& getCollections();
			EntityCollection& getCollection(size_t index);

			inline graphics::RenderCamera& getRenderCamera() { return m_sceneCamera; }

			// ----------------------------------------------------
			// SCENE MEMBERS
			// ----------------------------------------------------
			
		private:
			friend class Entity;
			friend class EntityCollection;

			std::string m_name{ "Empty Scene" };
			SceneRegistry m_sceneRegistry;

			EntityContainer m_container;

			graphics::RenderCamera m_sceneCamera;
			maths::vec3 m_backgroundColor{ 0.22f, 0.69f, 0.87f };
		};


} }


#endif // !MAR_ENGINE_ECS_SCENE_H