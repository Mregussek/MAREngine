/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_SCENE_H
#define MAR_ENGINE_SCENE_H

#include "../../mar.h"
#include "Component/Components.h"
#include "System/Systems.h"


namespace mar {
	namespace ecs {

		class Entity;


		class Scene {
			friend class Entity;

			const char* m_name{ "Empty Scene" };

			std::vector<Entity> m_entities;
			entt::registry m_registry;

		public:
			Scene(const char* name);
			~Scene();

			void update();

			Entity createEntity();

			// --- GET METHODS --- //

			inline std::vector<Entity>& getEntities() { return m_entities; }
			inline const char* getName() const { return m_name; }

			// --- SET METHODS --- //

			void setName(const char* name);
		};


} }


#endif // !MAR_ENGINE_SCENE_H