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

#include "../graphics/Renderer/RenderCamera.h"


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

			void destroyEntity(const int32_t& index);

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
			
			graphics::RenderCamera scene_camera;
			std::vector<Entity> entities;

			/* updated RenderableComponent in at least one entity / deleted Color/Texture Component!
			(It means that, we should reload vertices in VertexBuffer and Indices in ElementBuffer)
			(If no texture is bounded to entity, there is nothing to draw!) */
			bool updatedBuffers; 
			bool updatedTransforms;
			bool updatedCamera;
			bool updatedColors;
			bool updatedTextures2D;
			bool updatedTexturesCubemap;
			bool updatedLight;

			bool useEditorCamera;
		};


} }


#endif // !MAR_ENGINE_SCENE_H