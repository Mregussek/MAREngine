/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "Scene.h"
#include "Entity.h"
#include "../../Editor/Filesystem/EditorFilesystem.h"



namespace mar {
	namespace ecs {


		Scene::Scene(std::string name)
			: m_name(name),
			m_EditorMode(true),
			m_PauseMode(false)
		{
			m_registry = entt::registry();

			ECS_INFO("SCENE: scene is created, with entt::registry! (called constructor)");
		}

		void Scene::shutdown() {
			m_texture.shutdown();
			m_registry.clear();
			
			ECS_INFO("SCENE: registry is cleared! (called destructor)");
		}

		// -------------------------------------------------------------
		// ENTITIES MANAGEMENT
		// -------------------------------------------------------------

		Entity& Scene::createEntity() {
			Entity entity{ this };

			entity.addDefault();
			entity.addComponent<TagComponent>(ECS_TAG);

			entities.push_back(entity);

			ECS_INFO("SCENE: created entity!");

			return entities[entities.size() - 1];
		}

		void Scene::destroyEntity(const int32_t& index) {
			if (entities[index].isValid()) {
				entities[index].destroyYourself();
				entities.erase(entities.begin() + index);
			}

			ECS_INFO("SCENE: destroyed entity!");
		}

		// -------------------------------------------------------------
		// EDITOR MODE
		// -------------------------------------------------------------

		void Scene::initializeEditorMode() {
			resetStorages(m_colors, m_textures, m_cubemaps, m_light);

			for (auto& entity : entities) {
				auto& tran = entity.getComponent<TransformComponent>();

				if (entity.hasComponent<LightComponent>()) {
					auto& light = entity.getComponent<LightComponent>();

					m_light.positions.push_back(tran.center);
					m_light.components.push_back(light);
				}

				if (entity.hasComponent<CameraComponent>()) {
					typedef maths::Trig trig;

					auto& cam = entity.getComponent<CameraComponent>();

					if (cam.id.find("main") == std::string::npos) 
						continue;

					calculateCameraTransforms(tran, cam, scene_camera);
				}

				if (entity.hasComponent<ScriptComponent>()) {
					auto& script = entity.getComponent<ScriptComponent>();
					script.source = editor::Filesystem::loadPyScript(script.script.c_str());
				}

				if (!entity.hasComponent<RenderableComponent>())
					continue;

				if (entity.hasComponent<ColorComponent>()) {
					auto& ren = entity.getComponent<RenderableComponent>();
					auto& color = entity.getComponent<ColorComponent>();

					m_colors.transforms.push_back(tran.transform);
					submitVerticesIndices<maths::vec3>(ren, m_colors);
					submitSampler<maths::vec3>(color.texture, m_colors);

					ECS_TRACE("SCENE: initializing color entity!");
				}
				else if (entity.hasComponent<Texture2DComponent>()) {
					auto& ren = entity.getComponent<RenderableComponent>();
					auto& texture = entity.getComponent<Texture2DComponent>();

					m_texture.loadTexture(texture.texture);
					m_textures.paths.push_back(texture.texture);
					m_textures.transforms.push_back(tran.transform);
					submitVerticesIndices<int32_t>(ren, m_textures);
					submitSampler<int32_t>(m_textures.counter, m_textures);

					ECS_TRACE("SCENE: initializing texture2d entity!");
				}
				else if (entity.hasComponent<TextureCubemapComponent>()) {
					auto& ren = entity.getComponent<RenderableComponent>();
					auto& cubemap = entity.getComponent<TextureCubemapComponent>();

					m_texture.loadCubemap(cubemap.texture);
					m_cubemaps.paths.push_back(cubemap.texture);
					m_cubemaps.transforms.push_back(tran.transform);
					submitVerticesIndices<int32_t>(ren, m_cubemaps);
					submitSampler<int32_t>(m_cubemaps.counter, m_cubemaps);

					ECS_TRACE("SCENE: initializing cubemap entity!");
				}
			}
		}

		void Scene::updateEditorMode() {
			if (useEditorCamera) {
				auto& camdata = editor::Camera::getCameraData();
				scene_camera.model = camdata.model;
				scene_camera.view = camdata.view;
				scene_camera.projection = camdata.projection;
				scene_camera.mvp = camdata.mvp;
				scene_camera.position = camdata.position;
			}
			else {
				auto view = getView<CameraComponent>();
				for (auto entity : view) {
					auto& cam = getComponent<CameraComponent>(entity);

					if (cam.id.find("main") == std::string::npos) continue;

					auto& tran = getComponent<TransformComponent>(entity);
					calculateCameraTransforms(tran, cam, scene_camera);
				}
			}

			resetStorages(m_colors, m_textures, m_cubemaps, m_light);

			for (auto& entity : entities) {
				auto& tran = entity.getComponent<TransformComponent>();

				if (entity.hasComponent<LightComponent>()) {
					auto& light = entity.getComponent<LightComponent>();

					m_light.positions.push_back(tran.center);
					m_light.components.push_back(light);
				}

				if (!entity.hasComponent<RenderableComponent>())
					continue;

				if (entity.hasComponent<ColorComponent>()) {
					auto& ren = entity.getComponent<RenderableComponent>();
					auto& color = entity.getComponent<ColorComponent>();

					m_colors.transforms.push_back(tran.transform);
					submitVerticesIndices<maths::vec3>(ren, m_colors);
					submitSampler<maths::vec3>(color.texture, m_colors);

					ECS_TRACE("SCENE: initializing color entity!");
				}
				
				if (entity.hasComponent<Texture2DComponent>()) {
					auto& ren = entity.getComponent<RenderableComponent>();
					auto& texture = entity.getComponent<Texture2DComponent>();

					m_texture.loadTexture(texture.texture);
					m_textures.paths.push_back(texture.texture);
					m_textures.transforms.push_back(tran.transform);
					submitVerticesIndices<int32_t>(ren, m_textures);
					submitSampler<int32_t>(m_textures.counter, m_textures);

					ECS_TRACE("SCENE: initializing texture2d entity!");
				}
				
				if (entity.hasComponent<TextureCubemapComponent>()) {
					auto& ren = entity.getComponent<RenderableComponent>();
					auto& cubemap = entity.getComponent<TextureCubemapComponent>();

					m_texture.loadCubemap(cubemap.texture);
					m_cubemaps.paths.push_back(cubemap.texture);
					m_cubemaps.transforms.push_back(tran.transform);
					submitVerticesIndices<int32_t>(ren, m_cubemaps);
					submitSampler<int32_t>(m_cubemaps.counter, m_cubemaps);

					ECS_TRACE("SCENE: initializing cubemap entity!");
				}
			}
		}

		// -------------------------------------------------------------
		// PLAY MODE
		// -------------------------------------------------------------

		void Scene::initializePlayMode() {
			play_colors = m_colors;
			play_textures = m_textures;
			play_cubemaps = m_cubemaps;
			play_light = m_light;

			{
				auto view = getView<CameraComponent>();
				for (auto entity : view) {
					auto& cam = getComponent<CameraComponent>(entity);

					if (cam.id.find("main") == std::string::npos) continue;

					auto& tran = getComponent<TransformComponent>(entity);
					calculateCameraTransforms(tran, cam, scene_camera);
				}
			}
			
			{
				for (auto& entity : entities) {
					if (entity.hasComponent<ecs::ScriptComponent>()) {
						auto& sc = entity.getComponent<ScriptComponent>();
						auto from = System::changeSlashesToDots(sc.script);
						auto mod = System::getModuleFromPath(sc.script);

						sc.script_embed.loadScript(from.c_str(), mod.c_str());
						auto& module = sc.script_embed.getModule();
						module.attr("m_entityHandle") = pybind11::cast(entity.m_entityHandle);
						module.attr("m_scene") = pybind11::cast(entity.m_scene);
						module.attr("start")();
					}
				}
			}
		}

		void Scene::updatePlayMode() {
			{
				namespace py = pybind11;

				for (auto entity : entities) {
					if (entity.hasComponent<ScriptComponent>()) {
						auto& sc = entity.getComponent<ScriptComponent>();
						auto& module = sc.script_embed.getModule();
						module.attr("update")();
					}
				}
			}

		}

		void Scene::resetStorages(SceneStorage<maths::vec3>& s1, SceneStorage<int32_t>& s2, SceneStorage<int32_t>& s3, LightStorage& l1) {
			s1.vertices.clear();
			s1.indices.clear();
			s1.transforms.clear();
			s1.samplers.clear();
			s1.counter = 0;
			s1.indicesMax = 0;

			s2.vertices.clear();
			s2.indices.clear();
			s2.transforms.clear();
			s2.samplers.clear();
			s2.paths.clear();
			s2.counter = 0;
			s2.indicesMax = 0;

			s3.vertices.clear();
			s3.indices.clear();
			s3.transforms.clear();
			s3.samplers.clear();
			s3.paths.clear();
			s3.counter = 0;
			s3.indicesMax = 0;

			l1.components.clear();
			l1.positions.clear();

			ECS_TRACE("SCENE: called resetStorages method!");
		}

		template<typename T>
		void Scene::submitVerticesIndices(RenderableComponent& ren, SceneStorage<T>& storage) {
			std::vector<uint32_t> copy = ren.indices;

			graphics::ShapeManipulator::extendShapeID(ren.vertices, storage.stride, (float)storage.counter);
			graphics::ShapeManipulator::extendIndices(copy, storage.indicesMax);

			storage.vertices.insert(storage.vertices.end(), ren.vertices.begin(), ren.vertices.end());
			storage.indices.insert(storage.indices.end(), copy.begin(), copy.end());

			storage.indicesMax += ren.vertices.size() / storage.stride;

			ECS_TRACE("SCENE: submitted renderable component!");
		}

		template<typename T>
		void Scene::submitSampler(T& sampler, SceneStorage<T>& storage) {
			storage.samplers.push_back(sampler);
			storage.counter++;

			ECS_TRACE("SCENE: submitted sampler component!");
		}

		void Scene::calculateCameraTransforms(TransformComponent& tran, CameraComponent& cam, graphics::RenderCamera& ren_cam) {
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
				scene_camera.projection = maths::mat4::perspective(
					trig::toRadians(cam.p_fov), cam.p_aspectRatio, cam.p_near, cam.p_far
				);
			}
			else {
				scene_camera.projection = maths::mat4::orthographic(
					cam.o_left, cam.o_right, cam.o_top, cam.o_bottom, cam.o_near, cam.o_far
				);
			}

			scene_camera.mvp = scene_camera.projection * scene_camera.view * scene_camera.model;
		}


} }