/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_SCENE_H
#define MAR_ENGINE_SCENE_H

#include "../../mar.h"
#include "Components.h"
#include "Systems.h"
#include "ECSLogs.h"


namespace mar {
	namespace ecs {


		class Entity;


		class Scene {
			friend class Entity;

			std::string m_name{ "Empty Scene" };
			entt::registry m_registry;

		public:
			Scene(std::string name);
			~Scene();

			Entity& createEntity();

			// --- GET METHODS --- //

			inline const std::string& getName() const { return m_name; }

			template<typename T>
			auto getView() ->decltype(m_registry.view<T>()) {
				return m_registry.view<T>();
			}

			template<typename T>
			T& getComponent(entt::entity& entity) {
				return m_registry.get<T>(entity);
			}

			// --- SET METHODS --- //

			void setName(std::string name);
		
			std::vector<Entity> entities;
			bool updatedBuffers;
			bool updatedTransforms;
			bool updatedColors;
			bool updatedTextures2D;
			bool updatedTexturesCubemap;
			bool updatedLight;
		};


} }


#endif // !MAR_ENGINE_SCENE_H