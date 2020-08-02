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
			std::string m_name{ "Empty Scene" };
			entt::registry m_registry;

		public:
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

			Scene(std::string name);
			~Scene();

			Entity& createEntity();
			void destroyEntity(const int32_t& index);

			void update();

			// --- GET METHODS --- //

			inline const std::string& getName() const { return m_name; }

			// --- SET METHODS --- //

			void setName(std::string name);
			
			graphics::RenderCamera scene_camera;
			std::vector<Entity> entities;

		private:
			template<typename T>
			auto getView() ->decltype(m_registry.view<T>()) {
				return m_registry.view<T>();
			}

			template<typename T>
			T& getComponent(entt::entity& entity) {
				return m_registry.get<T>(entity);
			}

			friend class Entity;
		};


} }


#endif // !MAR_ENGINE_SCENE_H