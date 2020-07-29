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

			entt::registry m_registry;

		public:
			Scene(const char* name);
			~Scene();

			Entity createEntity();

			// --- GET METHODS --- //

			inline const char* getName() const { return m_name; }

			template<typename T>
			auto getView() ->decltype(m_registry.view<T>()) {
				return m_registry.view<T>();
			}

			template<typename T>
			T& getComponent(entt::entity entity) {
				return m_registry.get<T>(entity);
			}

			// --- SET METHODS --- //

			void setName(const char* name);
		
			std::vector<Entity> entities;
			bool updatedTransforms;
			bool updatedColors;
			bool updatedTextures2D;
			bool updatedTexturesCubemap;
			bool updatedLight;
		};


} }


#endif // !MAR_ENGINE_SCENE_H