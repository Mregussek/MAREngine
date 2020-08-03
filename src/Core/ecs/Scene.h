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
#include "../graphics/Mesh/Manipulators/ShapeManipulator.h"
#include "../graphics/Renderer/Texture/TextureOpenGL.h"


namespace mar {
	namespace ecs {

		class Entity;


		template<typename T>
		struct SceneStorage {
			std::vector<float> vertices;
			std::vector<uint32_t> indices;
			std::vector<maths::mat4> transforms;
			std::vector<std::string> paths; // only for textures!
			std::vector<T> samplers;
			int32_t counter;
			uint32_t indicesMax;
			static const int32_t stride = 3 + 3 + 2 + 1;
		};

		struct LightStorage {
			std::vector<maths::vec3> positions;
			std::vector<LightComponent> components;
		};

		class Scene {
			std::string m_name{ "Empty Scene" };
			entt::registry m_registry;

			LightStorage m_light;
			SceneStorage<maths::vec3> m_colors;
			SceneStorage<int32_t> m_textures;
			SceneStorage<int32_t> m_cubemaps;
			graphics::TextureOpenGL m_texture;

		public:
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

			Scene(std::string name);
			~Scene();

			Entity& createEntity();
			void destroyEntity(const int32_t& index);

			void initialize();
			void update();

			void resetStorages();

			template<typename T>
			void submitVerticesIndices(RenderableComponent& ren, SceneStorage<T>& storage);

			template<typename T>
			void submitSampler(T& sampler, SceneStorage<T>& storage);

			// --- GET METHODS --- //

			inline const std::string& getName() const { return m_name; }

			inline const SceneStorage<maths::vec3>& getColorsStorage() const { return m_colors; }
			inline const SceneStorage<int32_t>& getTexturesStorage() const { return m_textures; }
			inline const SceneStorage<int32_t>& getCubemapStorage() const { return m_cubemaps; }
			inline const graphics::RenderCamera& getRenderCamera() const { return scene_camera; }
			inline const LightStorage& getLightStorage() const { return m_light; }
			inline const graphics::TextureOpenGL& getTextureInstance() const { return m_texture; }

			// --- SET METHODS --- //

			void setName(std::string name);
			
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