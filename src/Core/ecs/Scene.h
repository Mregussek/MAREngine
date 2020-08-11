/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_SCENE_H
#define MAR_ENGINE_SCENE_H

#include "../../mar.h"
#include "ECS/Components.h"
#include "ECS/Systems.h"
#include "ECSLogs.h"

#include "../../Editor/Camera/Camera.h"
#include "../graphics/Renderer/RenderCamera.h"
#include "../graphics/Mesh/Manipulators/ShapeManipulator.h"


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
			int32_t counter = 0;
			uint32_t indicesMax = 0;
			static const int32_t stride = 3 + 3 + 2 + 1;
		};

		struct LightStorage {
			std::vector<maths::vec3> positions;
			std::vector<LightComponent> components;
		};

		struct PlayStorage {
			std::vector<TransformComponent> transforms;
			std::vector<ColorComponent> colors;
			std::vector<LightComponent> lights;
		};


		class Scene {
			// ----------------------------------------------------
			// SCENE PUBLIC METHODS
			// ----------------------------------------------------

		public:
			Scene(std::string name);
			void shutdown();

			Entity& createEntity();
			void destroyEntity(const int32_t& index);

			void initialize();
			void update() { if (m_EditorMode) updateEditorMode(); else { if(!m_PauseMode) updatePlayMode(); } }

			// --- GET METHODS --- //

			inline const std::string& getName() const { return m_name; }

			inline const SceneStorage<maths::vec3>& getColorsStorage() const { return m_colors; }
			inline const SceneStorage<int32_t>& getTexturesStorage() const { return m_textures; }
			inline const SceneStorage<int32_t>& getCubemapStorage() const { return m_cubemaps; }
			inline const LightStorage& getLightStorage() const { return m_light; }
			inline graphics::RenderCamera& getRenderCamera() { return scene_camera; }

			bool isEditorMode() { return m_EditorMode; }
			bool isPlayMode() { return !m_EditorMode; }
			bool isPauseMode() { return m_PauseMode; }

			// --- SET METHODS --- //

			void setName(std::string name) { m_name = name; }
			void setEditorMode() { m_EditorMode = true; }
			void setPlayMode() { m_EditorMode = false; initPlayMode(); }
			void setExitPlayMode() { m_EditorMode = true; unsetPauseMode(); exitPlayMode(); }
			void setPauseMode() { m_PauseMode = true; }
			void unsetPauseMode() { m_PauseMode = false; }
			
			// ----------------------------------------------------
			// SCENE PRIVATE METHODS
			// ----------------------------------------------------

		private:

			void initPlayMode();
			void exitPlayMode();

			void updateEditorMode();
			void updatePlayMode();

			template<typename T>
			auto getView() ->decltype(entt::registry::view<T>()) { return m_registry.view<T>(); }

			template<typename T>
			T& getComponent(entt::entity& entity) { return m_registry.get<T>(entity); }

			void resetStorages(SceneStorage<maths::vec3>& s1, SceneStorage<int32_t>& s2, SceneStorage<int32_t>& s3, LightStorage& l1);

			template<typename T>
			void submitVerticesIndices(RenderableComponent& ren, SceneStorage<T>& storage);

			template<typename T>
			void submitSampler(T& sampler, SceneStorage<T>& storage);

			void calculateCameraTransforms(TransformComponent& tran, CameraComponent& cam, graphics::RenderCamera& ren_cam);
		
			// ----------------------------------------------------
			// SCENE MEMBERS
			// ----------------------------------------------------

		public:
			std::vector<Entity> entities;
			bool useEditorCamera;

		private:
			friend class Entity;

			std::string m_name{ "Empty Scene" };

			entt::registry m_registry;

			LightStorage m_light;
			SceneStorage<maths::vec3> m_colors;
			SceneStorage<int32_t> m_textures;
			SceneStorage<int32_t> m_cubemaps;
			PlayStorage m_playstorage;

			graphics::RenderCamera scene_camera;

			bool m_EditorMode;
			bool m_PauseMode;
		};


} }


#endif // !MAR_ENGINE_SCENE_H