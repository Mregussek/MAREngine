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

#include "../../Editor/Camera/Camera.h"
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

		enum class ModificationStorage {
			NOT, COLORS, TEXTURES, CUBEMAPS
		};

#define MODIFIED_NOT ::mar::ecs::ModificationStorage::NOT
#define MODIFIED_COLORS ::mar::ecs::ModificationStorage::COLORS
#define MODIFIED_TEXTURES ::mar::ecs::ModificationStorage::TEXTURES
#define MODIFIED_CUBEMAPS ::mar::ecs::ModificationStorage::CUBEMAPS

		class Scene {
			std::string m_name{ "Empty Scene" };
			entt::registry m_registry;

			LightStorage m_light;
			SceneStorage<maths::vec3> m_colors;
			SceneStorage<int32_t> m_textures;
			SceneStorage<int32_t> m_cubemaps;
			graphics::TextureOpenGL m_texture;

			LightStorage play_light;
			SceneStorage<maths::vec3> play_colors;
			SceneStorage<int32_t> play_textures;
			SceneStorage<int32_t> play_cubemaps;

			bool m_EditorMode;
			bool m_PauseMode;

		public:
			graphics::RenderCamera scene_camera;
			std::vector<Entity> entities;

			bool useEditorCamera;

			Scene(std::string name);
			void shutdown();

			Entity& createEntity();
			void destroyEntity(const int32_t& index);

			void initialize() {
				if (m_EditorMode)
					initializeEditorMode();
				else
					initializePlayMode();
			}

			void update() {
				if (m_EditorMode)
					updateEditorMode();
				else {
					if(!m_PauseMode)
						updatePlayMode();
				}
			}

			// --- GET METHODS --- //

			inline const std::string& getName() const { return m_name; }

			inline const SceneStorage<maths::vec3>& getColorsStorage() const { return m_EditorMode ? m_colors : play_colors; }
			inline const SceneStorage<int32_t>& getTexturesStorage() const { return m_EditorMode ? m_textures : play_textures; }
			inline const SceneStorage<int32_t>& getCubemapStorage() const { return m_EditorMode ? m_cubemaps : play_cubemaps; }
			inline const LightStorage& getLightStorage() const { return m_EditorMode ? m_light : play_light; }
			inline const graphics::RenderCamera& getRenderCamera() const { return scene_camera; }
			inline const graphics::TextureOpenGL& getTextureInstance() const { return m_texture; }

			bool isEditorMode() { return m_EditorMode; }
			bool isPlayMode() { return !m_EditorMode; }
			bool isPauseMode() { return m_PauseMode; }

			// --- SET METHODS --- //

			void setName(std::string name) { m_name = name; }

			void setPlayMode() { 
				m_EditorMode = false; 
				initializePlayMode();
			}
			void setPauseDuringPlay() { m_PauseMode = true; }
			void setUnpauseDuringPlay() { m_PauseMode = false; }
			void stopPlayMode() { setEditorMode(); }
			void setEditorMode() { 
				m_EditorMode = true; 
				resetStorages(play_colors, play_textures, play_cubemaps, play_light);
			}
			
		private:

			void initializeEditorMode();
			void updateEditorMode();

			void initializePlayMode();
			void updatePlayMode();

			void resetStorages(SceneStorage<maths::vec3>& s1, SceneStorage<int32_t>& s2, SceneStorage<int32_t>& s3, LightStorage& l1);

			template<typename T>
			auto getView() ->decltype(m_registry.view<T>()) {
				return m_registry.view<T>();
			}

			template<typename T>
			T& getComponent(entt::entity& entity) {
				return m_registry.get<T>(entity);
			}

			template<typename T>
			void submitVerticesIndices(RenderableComponent& ren, SceneStorage<T>& storage);

			template<typename T>
			void submitSampler(T& sampler, SceneStorage<T>& storage);

			void calculateCameraTransforms(TransformComponent& tran, CameraComponent& cam, graphics::RenderCamera& ren_cam);

			friend class Entity;
		};


} }


#endif // !MAR_ENGINE_SCENE_H