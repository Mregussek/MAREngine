/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_SCENE_H
#define MAR_ENGINE_SCENE_H

#include "../../mar.h"
#include "Component/Components.h"


namespace mar {
	namespace ecs {

		class Entity;


		class Scene {
			friend class Entity;

			entt::registry m_registry;

		public:
			Scene();
			~Scene();

			void update();

			Entity createEntity();

			void clear();
		};


} }


#endif // !MAR_ENGINE_SCENE_H