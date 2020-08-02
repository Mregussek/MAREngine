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

		void Scene::update() {
			typedef maths::Trig trig;
			static maths::vec3 front;

			auto view = getView<CameraComponent>();
			for (auto entity : view) {
				auto& cam = getComponent<CameraComponent>(entity);

				if (cam.id.find("main") == std::string::npos) continue;

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
						trig::toRadians(cam.p_fov),
						cam.p_aspectRatio,
						cam.p_near,
						cam.p_far
					);
				}
				else {
					scene_camera.projection = maths::mat4::orthographic(
						cam.o_left,
						cam.o_right,
						cam.o_top,
						cam.o_bottom,
						cam.o_near,
						cam.o_far
					);
				}
			}
		}


} }