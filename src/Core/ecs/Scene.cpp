/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "Scene.h"
#include "Entity.h"


namespace mar {
	namespace ecs {


		Scene::Scene(std::string name)
			: m_name(name),
			updatedTransforms(false),
			updatedColors(false),
			updatedTextures2D(false),
			updatedTexturesCubemap(false),
			updatedLight(false),
			updatedBuffers(false),
			updatedCamera(false)
		{
			m_registry = entt::registry();

			ECS_INFO("SCENE: scene is created, with entt::registry! (called constructor)");
		}

		Scene::~Scene() {
			m_registry.clear();
			
			ECS_INFO("SCENE: registry is cleared! (called destructor)");
		}

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

		void Scene::setName(std::string name) {
			m_name = name;
		}

		void Scene::initialize() {
			resetStorages();

			for (auto& entity : entities) {
				auto& tran = entity.getComponent<TransformComponent>();

				if (entity.hasComponent<ColorComponent>()) {
					auto& ren = entity.getComponent<RenderableComponent>();
					auto& color = entity.getComponent<ColorComponent>();

					m_colors.transforms.push_back(tran.transform);
					submitVerticesIndices<maths::vec3>(ren, m_colors);
					submitSampler<maths::vec3>(color.color, m_colors);

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

					m_texture.loadCubemap(cubemap.cubemap);
					m_cubemaps.paths.push_back(cubemap.cubemap);
					m_cubemaps.transforms.push_back(tran.transform);
					submitVerticesIndices<int32_t>(ren, m_cubemaps);
					submitSampler<int32_t>(m_cubemaps.counter, m_cubemaps);

					ECS_TRACE("SCENE: initializing cubemap entity!");
				}

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

					maths::vec3 front{
						trig::cosine(trig::toRadians(tran.angles.y)) * trig::cosine(trig::toRadians(tran.angles.x)),
						trig::sine(trig::toRadians(tran.angles.x)),
						trig::sine(trig::toRadians(tran.angles.y)) * trig::cosine(trig::toRadians(tran.angles.x))
					};

					front = maths::vec3::normalize(front);

					scene_camera.position = tran.center;
					scene_camera.model = maths::mat4::translation({ 0.f, 0.f, 0.f });
					scene_camera.view = maths::mat4::lookAt(
						tran.center, tran.center + front, { 0.f, 1.0f, 0.f }
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
				}


			}
		}

		void Scene::update() {
			typedef maths::Trig trig;
			static maths::vec3 front;

			auto view = getView<CameraComponent>();
			for (auto entity : view) {
				auto& cam = getComponent<CameraComponent>(entity);

				if (cam.id.find("main") == std::string::npos) 
					continue;

				auto& tran = getComponent<TransformComponent>(entity);

				front.x = trig::cosine(trig::toRadians(tran.angles.y)) * trig::cosine(trig::toRadians(tran.angles.x));
				front.y = trig::sine(trig::toRadians(tran.angles.x));
				front.z = trig::sine(trig::toRadians(tran.angles.y)) * trig::cosine(trig::toRadians(tran.angles.x));
				front = maths::vec3::normalize(front);

				scene_camera.position = tran.center;
				scene_camera.model = maths::mat4::translation({ 0.f, 0.f, 0.f });
				scene_camera.view = maths::mat4::lookAt(
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
		
		
		
		}

		void Scene::resetStorages() {
			m_colors.vertices.clear();
			m_colors.indices.clear();
			m_colors.transforms.clear();
			m_colors.samplers.clear();
			m_colors.counter = 0;
			m_colors.indicesMax = 0;

			m_textures.vertices.clear();
			m_textures.indices.clear();
			m_textures.transforms.clear();
			m_textures.samplers.clear();
			m_textures.counter = 0;
			m_textures.indicesMax = 0;

			m_cubemaps.vertices.clear();
			m_cubemaps.indices.clear();
			m_cubemaps.transforms.clear();
			m_cubemaps.samplers.clear();
			m_cubemaps.counter = 0;
			m_cubemaps.indicesMax = 0;

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


} }