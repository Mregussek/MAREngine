/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "SceneManager.h"
#include "ECS/Entity.h"
#include "../../Editor/Filesystem/EditorFilesystem.h"
#include "ECS/Components.h"
#include "../graphics/Renderer/RenderCamera.h"
#include "../../Editor/Camera/Camera.h"
#include "ECS/Systems.h"
#include "../graphics/Mesh/Manipulators/ShapeManipulator.h"
#include "SceneEvents.h"
#include "../../Window/Window.h"


namespace mar {
	namespace ecs {


		SceneManager::SceneManager()
			: useEditorCamera(true),
			m_EditorMode(true),
			m_PauseMode(false),
			m_scene(nullptr)
		{
			SceneEvents::Instance().scene_manager = this;

			ECS_INFO("SCENE_MANAGER: called constructor");
		}

		void SceneManager::initialize() {
			ECS_TRACE("SCENE_MANAGER: going to initialize!");

			m_sceneStorage.resetAll();

			for (auto& entity : m_scene->getEntities()) {
				auto& tran = entity.getComponent<TransformComponent>();

				if (entity.hasComponent<LightComponent>()) {
					auto& light = entity.getComponent<LightComponent>();
					m_sceneStorage.pushLight(tran.center, light);
				}

				if (!useEditorCamera)
					if (entity.hasComponent<CameraComponent>()) {
						submitCamera(entity, tran);
					}

				if (!entity.hasComponent<RenderableComponent>()) // we wan't push any entities to draw, if there is not RenderableComponent
					continue;

				auto& ren = entity.getComponent<RenderableComponent>();

				if (entity.hasComponent<ColorComponent>()) {
					submitColorEntity(entity, tran, ren, m_sceneStorage.colors_storage);
				}
				else if (entity.hasComponent<TextureCubemapComponent>()) {
					submitTextureEntity<TextureCubemapComponent>(entity, tran, ren, m_sceneStorage.cubemap_storage, 2);
				}
				else if (entity.hasComponent<Texture2DComponent>()) {
					submitTextureEntity<Texture2DComponent>(entity, tran, ren, m_sceneStorage.texture_storage, 1);
				}
			}

			ECS_INFO("SCENE_MANAGER: initialized!");
		}

		void SceneManager::update() {
			ECS_TRACE("SCENE_MANAGER: going to update");

			if (m_EditorMode) {
				updateEditorMode();
			}
			else {
				if (!m_PauseMode)
					updatePlayMode();
				else
					updatePauseMode();
			}

			ECS_INFO("SCENE_MANAGER: updated!");
		}

		// -------------------------------------------------------------
		// EDITOR MODE
		// -------------------------------------------------------------

		void SceneManager::updateEditorMode() {
			ECS_TRACE("SCENE_MANAGER: updating editor mode");

			if (useEditorCamera) {
				auto& camdata = editor::Camera::getCameraData();
				auto& cam = m_scene->getRenderCamera();

				cam = camdata;

				ECS_TRACE("SCENE: initializing editor camera on scene");
			}
			else {
				for (auto& entity : m_scene->getEntities()) {
					if (entity.hasComponent<CameraComponent>()) {
						submitCamera(entity, entity.getComponent<TransformComponent>());
					}
				}
			}

			ECS_INFO("SCENE_MANAGER: updated editor mode!");
		}

		// -------------------------------------------------------------
		// PLAY MODE
		// -------------------------------------------------------------

		void SceneManager::initPlayMode() {
			ECS_TRACE("SCENE_MANAGER: going to initialize play mode");

			m_sceneStorage.reset(m_sceneStorage.play_storage);

			for (auto& entity : m_scene->getEntities()) {
				m_sceneStorage.play_storage.transforms.push_back(entity.getComponent<TransformComponent>());

				if (entity.hasComponent<LightComponent>())
					m_sceneStorage.play_storage.lights.push_back(entity.getComponent<LightComponent>());

				if (entity.hasComponent<ColorComponent>())
					m_sceneStorage.play_storage.colors.push_back(entity.getComponent<ColorComponent>());

				if (entity.hasComponent<ScriptComponent>()) {
					auto& sc = entity.getComponent<ScriptComponent>();
					std::string from = System::changeSlashesToDots(sc.script);
					std::string what = System::getModuleFromPath(sc.script);

					sc.ps.loadScript(from.c_str(), what.c_str());
					sc.ps.start(entity);
				}
			}

			ECS_INFO("SCENE_MANAGER: initialized play mode!");
		}

		template<typename T>
		void pop_front(std::vector<T>& vec) {
			MAR_CORE_ASSERT(!vec.empty(), "Cannot pop front, vec is empty!");
			vec.erase(vec.begin());
		}

		void SceneManager::exitPlayMode() {
			ECS_TRACE("SCENE_MANAGER: going to exit play mode");

			for (auto& entity : m_scene->getEntities()) {
				auto& tran = entity.getComponent<TransformComponent>();
				auto& tran_store = m_sceneStorage.play_storage.transforms.front();

				tran.center = tran_store.center;
				tran.angles = tran_store.angles;
				tran.scale = tran_store.scale;

				System::handleTransformComponent(tran);

				pop_front(m_sceneStorage.play_storage.transforms);

				if (entity.hasComponent<LightComponent>()) {
					auto& light = entity.getComponent<LightComponent>();
					auto& light_store = m_sceneStorage.play_storage.lights.front();

					light.ambient = light_store.ambient;
					light.diffuse = light_store.diffuse;
					light.specular = light_store.specular;
					light.quadratic = light_store.quadratic;
					light.linear = light_store.linear;
					light.shininess = light_store.shininess;
					light.constant = light_store.constant;

					pop_front(m_sceneStorage.play_storage.lights);
				}

				if (entity.hasComponent<ColorComponent>()) {
					auto& color = entity.getComponent<ColorComponent>();
					auto& color_store = m_sceneStorage.play_storage.colors.front();

					color.texture = color_store.texture;

					pop_front(m_sceneStorage.play_storage.colors);
				}
			}

			initialize();

			ECS_INFO("SCENE_MANAGER: exited play mode!");
		}

		void SceneManager::updatePlayMode() {
			ECS_TRACE("SCENE_MANAGER: going to update play mode");

			static size_t counter_color;
			static size_t counter_texture;
			static size_t counter_cubemap;
			static size_t counter_light;

			counter_color = 0;
			counter_texture = 0;
			counter_cubemap = 0;
			counter_light = 0;

			for (auto entity : m_scene->getEntities()) {
				auto& tran = entity.getComponent<TransformComponent>();

				if (entity.hasComponent<ScriptComponent>()) {
					auto& sc = entity.getComponent<ScriptComponent>();
					sc.ps.update(entity);

					if (entity.hasComponent<ColorComponent>()) {
						auto& color = entity.getComponent<ColorComponent>();
						m_sceneStorage.colors_storage.samplers[counter_color] = color.texture;
						m_sceneStorage.colors_storage.transforms[counter_color] = tran.transform;
						counter_color++;
					}

					if (entity.hasComponent<Texture2DComponent>()) {
						m_sceneStorage.texture_storage.transforms[counter_texture] = tran.transform;
						counter_texture++;
					}

					if (entity.hasComponent<TextureCubemapComponent>()) {
						m_sceneStorage.cubemap_storage.transforms[counter_cubemap] = tran.transform;
						counter_cubemap++;
					}

					if (entity.hasComponent<LightComponent>()) {
						auto& light = entity.getComponent<LightComponent>();
						m_sceneStorage.light_storage.positions[counter_light] = tran.center;
						m_sceneStorage.light_storage.components[counter_light] = light;
						counter_light++;
					}
				}
				else {
					if (entity.hasComponent<ColorComponent>()) counter_color++;
					if (entity.hasComponent<Texture2DComponent>()) counter_texture++;
					if (entity.hasComponent<TextureCubemapComponent>()) counter_cubemap++;
					if (entity.hasComponent<LightComponent>()) counter_light++;
				}

				if (entity.hasComponent<CameraComponent>()) {
					submitCamera(entity, tran);
				}
			}

			ECS_INFO("SCENE_MANAGER: updated play mode");
		}

		void SceneManager::updatePauseMode() {
			ECS_TRACE("SCENE_MANAGER: going to update pause mode");

			static size_t counter_color;
			static size_t counter_texture;
			static size_t counter_cubemap;
			static size_t counter_light;

			counter_color = 0;
			counter_texture = 0;
			counter_cubemap = 0;
			counter_light = 0;

			for (auto entity : m_scene->getEntities()) {
				auto& tran = entity.getComponent<TransformComponent>();

				if (entity.hasComponent<ColorComponent>()) {
					auto& color = entity.getComponent<ColorComponent>();
					m_sceneStorage.colors_storage.samplers[counter_color] = color.texture;
					m_sceneStorage.colors_storage.transforms[counter_color] = tran.transform;
					counter_color++;
				}

				if (entity.hasComponent<Texture2DComponent>()) {
					m_sceneStorage.texture_storage.transforms[counter_texture] = tran.transform;
					counter_texture++;
				}

				if (entity.hasComponent<TextureCubemapComponent>()) {
					m_sceneStorage.cubemap_storage.transforms[counter_cubemap] = tran.transform;
					counter_cubemap++;
				}

				if (entity.hasComponent<LightComponent>()) {
					auto& light = entity.getComponent<LightComponent>();
					m_sceneStorage.light_storage.positions[counter_light] = tran.center;
					m_sceneStorage.light_storage.components[counter_light] = light;
					counter_light++;
				}

				if (entity.hasComponent<CameraComponent>()) {
					submitCamera(entity, tran);
				}
			}

			ECS_INFO("SCENE_MANAGER: updated pause mode");
		}

		// -------------------------------------------------------------
		// OTHER METHODS - SCENE
		// -------------------------------------------------------------

		void SceneManager::submitColorEntity(const Entity& entity, TransformComponent& tran, RenderableComponent& ren, BufferStorage<maths::vec3>& storage) {
			ECS_TRACE("SCENE_MANAGER: going to submit color entity");
			
			auto& color = entity.getComponent<ColorComponent>();

			storage.transforms.push_back(tran.transform);
			submitVerticesIndices<maths::vec3>(ren, storage);
			submitSampler<maths::vec3>(color.texture, storage);

			ECS_TRACE("SCENE_MANAGER: submitted color entity!");
		}

		template<typename TextureType>
		void SceneManager::submitTextureEntity(const Entity& entity, TransformComponent& tran, RenderableComponent& ren, BufferStorage<int32_t>& storage, int32_t i) {
			ECS_TRACE("SCENE_MANAGER: going to submit texture entity");
			
			auto& texture = entity.getComponent<TextureType>();

			storage.paths.push_back(texture.texture);
			storage.textureType.push_back(i);
			storage.transforms.push_back(tran.transform);
			submitVerticesIndices<int32_t>(ren, storage);
			submitSampler<int32_t>(storage.counter, storage);

			ECS_TRACE("SCENE_MANAGER: submitted texture entity!");
		}

		template<typename T>
		void SceneManager::submitVerticesIndices(RenderableComponent& ren, BufferStorage<T>& storage) {
			ECS_TRACE("SCENE_MANAGER: going to submit renderable component (vertices / indices) - {}", ren.id);

			std::vector<uint32_t> copy = ren.indices;

			graphics::ShapeManipulator::extendShapeID(ren.vertices, storage.stride, (float)storage.counter);
			graphics::ShapeManipulator::extendIndices(copy, storage.indicesMax);

			storage.vertices.insert(storage.vertices.end(), ren.vertices.begin(), ren.vertices.end());
			storage.indices.insert(storage.indices.end(), copy.begin(), copy.end());

			storage.indicesMax += ren.vertices.size() / storage.stride;

			ECS_TRACE("SCENE: submitted renderable component - {}!", ren.id);
		}

		template<typename T>
		void SceneManager::submitSampler(T& sampler, BufferStorage<T>& storage) {
			ECS_TRACE("SCENE_MANAGER: going to submit sampler");

			storage.samplers.push_back(sampler);
			storage.counter++;

			ECS_TRACE("SCENE_MANAGER: submitted sampler component!");
		}

		void SceneManager::submitCamera(const Entity& entity, TransformComponent& tran) {
			ECS_TRACE("SCENE_MANAGER: going to submit camera");

			auto& cam = entity.getComponent<CameraComponent>();

			if (cam.id.find("main") == std::string::npos)
				return;

			calculateCameraTransforms(tran, cam, m_scene->getRenderCamera());

			ECS_TRACE("SCENE_MANAGER: submitted camera");
		}

		void SceneManager::calculateCameraTransforms(TransformComponent& tran, CameraComponent& cam, graphics::RenderCamera& ren_cam) {
			typedef maths::Trig trig;
			static maths::vec3 front;

			front.x = trig::cosine(trig::toRadians(tran.angles.y)) * trig::cosine(trig::toRadians(tran.angles.x));
			front.y = trig::sine(trig::toRadians(tran.angles.x));
			front.z = trig::sine(trig::toRadians(tran.angles.y)) * trig::cosine(trig::toRadians(tran.angles.x));
			front = maths::vec3::normalize(front);

			ren_cam.position = tran.center;
			ren_cam.model = maths::mat4::translation({ 0.f, 0.f, 0.f });
			ren_cam.view = maths::mat4::lookAt(
				tran.center,
				tran.center + front,
				{ 0.f, 1.0f, 0.f }
			);

			if (cam.Perspective) {
				ren_cam.projection = maths::mat4::perspective(
					trig::toRadians(cam.p_fov), cam.p_aspectRatio, cam.p_near, cam.p_far
				);
			}
			else {
				ren_cam.projection = maths::mat4::orthographic(
					cam.o_left, cam.o_right, cam.o_top, cam.o_bottom, cam.o_near, cam.o_far
				);
			}

			ren_cam.mvp = ren_cam.projection * ren_cam.view * ren_cam.model;
		
			ECS_TRACE("SCENE_MANAGER: calculated camera transform!");
		}




} }