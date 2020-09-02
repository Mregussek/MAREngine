/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "SceneManager.h"

#include "SceneEvents.h"

#include "ECS/Entity.h"
#include "ECS/Systems.h"

#include "../../Editor/Filesystem/EditorFilesystem.h"
#include "../../Editor/Camera/Camera.h"

#include "../graphics/Renderer/RenderCamera.h"
#include "../graphics/Mesh/ShapeManipulator.h"
#include "../graphics/Renderer/RenderPipeline.h"

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

		SceneManager::~SceneManager() {
			delete m_scene;

			ECS_INFO("SCENE_MANAGER: called destructor");
		}

		void SceneManager::initialize() {
			ECS_TRACE("SCENE_MANAGER: going to initialize!");

			auto& render_pipeline = graphics::RenderPipeline::getInstance();

			render_pipeline.reset();

			for (size_t i = 0; i < m_scene->getEntities().size(); i++) {
				auto& entity = m_scene->getEntity(i);

				auto& tran = entity.getComponent<TransformComponent>();
				
				if (entity.hasComponent<LightComponent>()) {
					auto& light = entity.getComponent<LightComponent>();
					render_pipeline.submitLight(tran.center, light);
				}

				if (entity.hasComponent<RenderableComponent>()) {
					render_pipeline.submitTransform(tran);

					auto& ren = entity.getComponent<RenderableComponent>();
					render_pipeline.submitRenderable(ren);

					if (entity.hasComponent<ColorComponent>()) {
						auto& color = entity.getComponent<ColorComponent>();
						render_pipeline.submitColor(ren.shader_id, color);
					}
					else if (entity.hasComponent<Texture2DComponent>()) {
						auto& tex = entity.getComponent<Texture2DComponent>();
						render_pipeline.submitTexture2D(ren.shader_id, tex);
					}
					else if (entity.hasComponent<TextureCubemapComponent>()) {
						auto& cube = entity.getComponent<TextureCubemapComponent>();
						render_pipeline.submitCubemap(ren.shader_id, cube);
					}
				}

				if (!useEditorCamera) {
					if (entity.hasComponent<CameraComponent>()) {
						submitCamera(tran, entity.getComponent<CameraComponent>());
					}
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
				auto camdata = editor::Camera::getCameraData();
				auto& cam = m_scene->getRenderCamera();

				cam = camdata;

				ECS_TRACE("SCENE: initializing editor camera on scene");
			}
			else {
				for (auto& entity : m_scene->getEntities()) {
					if (entity.hasComponent<CameraComponent>()) {
						submitCamera(entity.getComponent<TransformComponent>(), entity.getComponent<CameraComponent>());
					}
				}
			}

			graphics::RenderPipeline::getInstance().submitCamera(&m_scene->getRenderCamera());

			ECS_INFO("SCENE_MANAGER: updated editor mode!");
		}

		// -------------------------------------------------------------
		// PLAY MODE
		// -------------------------------------------------------------

		void SceneManager::initPlayMode() {
			ECS_TRACE("SCENE_MANAGER: going to initialize play mode");

			

			ECS_INFO("SCENE_MANAGER: initialized play mode!");
		}

		void SceneManager::exitPlayMode() {
			ECS_TRACE("SCENE_MANAGER: going to exit play mode");


			ECS_INFO("SCENE_MANAGER: exited play mode!");
		}

		void SceneManager::updatePlayMode() {
			ECS_TRACE("SCENE_MANAGER: going to update play mode");

			

			ECS_INFO("SCENE_MANAGER: updated play mode");
		}

		void SceneManager::updatePauseMode() {
			ECS_TRACE("SCENE_MANAGER: going to update pause mode");

			
			ECS_INFO("SCENE_MANAGER: updated pause mode");
		}

		// -------------------------------------------------------------
		// CAMERA STUFF
		// -------------------------------------------------------------

		void SceneManager::submitCamera(TransformComponent& tran, CameraComponent& cam) {
			if (cam.id.find("main") == std::string::npos)
				return;

			calculateCameraTransforms(tran, cam);

			ECS_TRACE("SCENE_MANAGER: submitted camera");
		}

		void SceneManager::calculateCameraTransforms(TransformComponent& tran, CameraComponent& cam) {
			typedef maths::Trig trig;
			static maths::vec3 front;

			front.x = trig::cosine(trig::toRadians(tran.angles.y)) * trig::cosine(trig::toRadians(tran.angles.x));
			front.y = trig::sine(trig::toRadians(tran.angles.x));
			front.z = trig::sine(trig::toRadians(tran.angles.y)) * trig::cosine(trig::toRadians(tran.angles.x));
			front = maths::vec3::normalize(front);

			auto& ren_cam = m_scene->getRenderCamera();

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