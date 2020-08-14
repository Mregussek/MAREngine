/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_ECS_SCENE_H
#define MAR_ENGINE_ECS_SCENE_H

#include "../../mar.h"
#include "ECS/Components.h"
#include "ECSLogs.h"

#include "../graphics/Renderer/RenderCamera.h"


namespace mar {
	namespace ecs {

		class Entity;


		class Scene {
			// ----------------------------------------------------
			// SCENE PUBLIC METHODS
			// ----------------------------------------------------

		public:
			Scene(std::string name);
			void shutdown();

			Entity& createEntity();
			void destroyEntity(const int32_t& index);

			// --- SET METHODS --- //

			void setName(std::string name) { m_name = name; }
			
			// --- GET METHODS --- //

			inline const std::string& getName() const { return m_name; }

			const std::vector<Entity>& getEntities() const { return entities; }
			Entity& getEntity(size_t index) { return entities[index]; }

			inline graphics::RenderCamera& getRenderCamera() { return scene_camera; }

			// ----------------------------------------------------
			// SCENE MEMBERS
			// ----------------------------------------------------
			
		private:
			friend class Entity;

			std::string m_name{ "Empty Scene" };
			entt::registry m_registry;
			std::vector<Entity> entities;
			graphics::RenderCamera scene_camera;
		};


} }


#endif // !MAR_ENGINE_ECS_SCENE_H