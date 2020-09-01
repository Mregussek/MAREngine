/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_ECS_SCENE_H
#define MAR_ENGINE_ECS_SCENE_H

#include "../../mar.h"
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

			static Scene* createEmptyScene(std::string name);

			Entity& createEntity();
			void destroyEntity(int32_t index);

			// --- SET METHODS --- //

			void setName(std::string name) { m_name = std::move(name); }
			void setBackground(maths::vec3 v) { m_backgroundColor = std::move(v); }
			
			// --- GET METHODS --- //

			inline const std::string& getName() const { return m_name; }
			inline maths::vec3& getBackground() { return m_backgroundColor; }

			const std::vector<Entity>& getEntities() const;
			Entity& getEntity(size_t index);

			inline graphics::RenderCamera& getRenderCamera() { return m_sceneCamera; }

			// ----------------------------------------------------
			// SCENE MEMBERS
			// ----------------------------------------------------
			
		private:
			friend class Entity;

			std::string m_name{ "Empty Scene" };
			entt::registry m_registry;
			std::vector<Entity> m_entities;
			graphics::RenderCamera m_sceneCamera;
			maths::vec3 m_backgroundColor{ 0.22f, 0.69f, 0.87f };
		};


} }


#endif // !MAR_ENGINE_ECS_SCENE_H