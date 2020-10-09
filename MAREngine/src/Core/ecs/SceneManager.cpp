/**
 *           MAREngine - open source 3D game engine
 * Copyright (C) 2020-present Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 * All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
**/


#include "SceneManager.h"

#include "ECSLogs.h"
#include "SceneEvents.h"
#include "Scene.h"

#include "Entity/EntityCollection.h"
#include "Entity/Entity.h"

#include "../../Editor/Camera/Camera.h"

#include "../graphics/Renderer/RenderCamera.h"
#include "../graphics/Renderer/RenderPipeline.h"


namespace mar {
	namespace ecs {


		SceneManager::SceneManager() {
			SceneEvents::Instance().scene_manager = this;

			ECS_INFO("SCENE_MANAGER: called constructor");
		}

		void SceneManager::initialize() {
			ECS_TRACE("SCENE_MANAGER: going to initialize!");

			auto& render_pipeline = graphics::RenderPipeline::getInstance();
			render_pipeline.reset();

			auto pushEntities = [this, &render_pipeline](const std::vector<Entity>& entitiesVector) {
				for (const auto& entity : entitiesVector) {
					auto& tran = entity.getComponent<TransformComponent>();

					render_pipeline.pushEntityToPipeline(entity);

					if (!useEditorCamera) {
						if (entity.hasComponent<CameraComponent>()) {
							submitCamera(tran, entity.getComponent<CameraComponent>());
						}
					}
				}
			};

			auto pushCollections = [this, &render_pipeline, &pushEntities]() {
				for (const auto& collection : m_scene->getCollections()) {
					pushEntities(collection.getEntities());
				}
			};

			pushEntities(m_scene->getEntities());
			pushCollections();

			ECS_INFO("SCENE_MANAGER: initialized!");
		}

		void SceneManager::shutdown() { 
			ECS_TRACE("SCENE_MANAGER: going to shutdown scene manager");

			m_scene->shutdown(); 
			delete m_scene;

			ECS_INFO("SCENE_MANAGER: called shutdown method");
		}

		void SceneManager::update() {
			ECS_TRACE("SCENE_MANAGER: going to update");

			if (m_EditorMode) { updateEditorMode(); }
			else {
				if (!m_PauseMode) { updatePlayMode(); }
				else { updatePauseMode(); }	
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

			m_playStorage.clear();

			for (auto& entity : m_scene->getEntities()) {
				m_playStorage.pushEntityToStorage(m_playStorage.entity_storage, entity);

				if (entity.hasComponent<ScriptComponent>()) {
					auto& sc = entity.getComponent<ScriptComponent>();
					sc.pythonScript.loadScript(sc.script);
					sc.pythonScript.start(&entity);
				}
			}

			for (auto& collection : m_scene->getCollections()) {
				m_playStorage.pushCollectionToStorage(m_playStorage.collection_storage, collection);

				for (auto& entity : collection.getEntities()) {
					if (entity.hasComponent<ScriptComponent>()) {
						auto& sc = entity.getComponent<ScriptComponent>();
						sc.pythonScript.loadScript(sc.script);
						sc.pythonScript.start(&entity);
					}
				}
			}

			ECS_INFO("SCENE_MANAGER: initialized play mode!");
		}

		void SceneManager::exitPlayMode() {
			ECS_TRACE("SCENE_MANAGER: going to exit play mode");

			for (size_t i = 0; i < m_scene->getEntities().size(); i++) {
				m_playStorage.loadEntityFromStorage(m_playStorage.entity_storage, m_scene->getEntity(i));
			}

			for (size_t i = 0; i < m_scene->getCollections().size(); i++) {
				m_playStorage.loadCollectionFromStorage(m_playStorage.collection_storage, m_scene->getCollection(i));
			}

			initialize();

			ECS_INFO("SCENE_MANAGER: exited play mode!");
		}

		void SceneManager::updatePlayMode() {
			ECS_TRACE("SCENE_MANAGER: going to update play mode");

			auto& render_pipeline = graphics::RenderPipeline::getInstance();

			for (size_t i = 0; i < m_scene->getEntities().size(); i++) {
				auto& entity = m_scene->getEntity(i);
				auto& tran = entity.getComponent<TransformComponent>();

				if (entity.hasComponent<ScriptComponent>()) {
					auto& rpc = entity.getComponent<RenderPipelineComponent>();
					auto& sc = entity.getComponent<ScriptComponent>();
					sc.pythonScript.update(&entity);

					if (entity.hasComponent<RenderableComponent>()) {
						render_pipeline.modifyTransform(tran, rpc.container_index, rpc.transform_index);
					}

					if (entity.hasComponent<LightComponent>()) {
						auto& light = entity.getComponent<LightComponent>();
						render_pipeline.modifyLight(tran.center, light, rpc.container_light_index, rpc.light_index);
					}

					if (entity.hasComponent<ColorComponent>()) {
						auto& color = entity.getComponent<ColorComponent>();
						render_pipeline.modifyColor(color, rpc.container_index, rpc.color_index);
					}
				}

				if (entity.hasComponent<CameraComponent>()) {
					auto& cam = entity.getComponent<CameraComponent>();
					submitCamera(tran, cam);
				}
			}

			for (auto& collection : m_scene->getCollections()) {
				/// TODO: add support for collection scripting
			}

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
			if (cam.id.find("main") == std::string::npos) return;

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

		// -------------------------------------------------------------
		// GET / SET
		// -------------------------------------------------------------

		void SceneManager::setScene(Scene* scene) { 
			m_scene = scene; 
		}

		Scene* SceneManager::getScene() { 
			return m_scene; 
		}


} }