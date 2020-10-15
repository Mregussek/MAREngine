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


#include "Filesystem_Loading.h"
#include "../../Core/graphics/Mesh/MeshCreator.h"
#include "../../Core/ecs/Scene.h"
#include "../../Core/ecs/Entity/Entity.h"
#include "../../Core/ecs/Entity/EntityCollection.h"
#include "../../Core/ecs/Components/Components.h"


namespace mar::editor {


	void Filesystem_Loading::loadScene(std::ifstream& file, ecs::Scene* scene) {
		std::string line;

		while (std::getline(file, line)) {
			if (line.find("#EntityStart") != std::string::npos) {
				const auto& entity = scene->createEntity();
				Filesystem_Loading::loadEntity(file, scene, entity);
			}
			else if (line.find("#EntityCollectionStart") != std::string::npos) {
				const auto& collection = scene->createCollection();
				Filesystem_Loading::loadCollection(file, scene, collection);
			}
			else if (line.find("#SceneEnd") != std::string::npos) {
				return;
			}
		}
	}

	void Filesystem_Loading::loadCollection(std::ifstream& file, ecs::Scene* scene, const ecs::EntityCollection& collection) {
		std::string line;

		while (std::getline(file, line)) {
			if (line.find("#EntityStart") != std::string::npos) {
				const auto& entity = collection.createEntity();
				loadEntity(file, scene, entity);
			}
			else if (line.find("#CollectionTagComponent") != std::string::npos) {
				std::istringstream is(line.substr(24));
				std::string new_tag;
				is >> new_tag;
				auto& tag = collection.getComponent<ecs::TagComponent>();
				tag = new_tag;
			}
			else if (line.find("#CollectionRenderableComponent") != std::string::npos) {
				std::istringstream is(line.substr(31));
				std::string path_to_load_obj;
				is >> path_to_load_obj;

				auto& crc = collection.addComponent<ecs::CollectionRenderableComponent>();
				crc.id = path_to_load_obj;

				graphics::MeshCreator::loadOBJ("CannotFindOBJname", path_to_load_obj, collection);

				auto size_after_loading = collection.getEntitiesCount();
				for (size_t i = 0; i < size_after_loading; ) {
					std::getline(file, line);
					if (line.find("#EntityStart") != std::string::npos) {
						const auto& entity = collection.getEntity(i);
						loadEntity(file, scene, entity);
						i++;
					}
				}
			}
			else if (line.find("#CollectionTransformComponent") != std::string::npos) {
				auto& tran = collection.getComponent<ecs::TransformComponent>();

				// #center - 7 letters
				std::getline(file, line);
				std::istringstream is(line.substr(7));
				float arr[3];
				is >> arr[0] >> arr[1] >> arr[2];

				tran.center.x = arr[0];
				tran.center.y = arr[1];
				tran.center.z = arr[2];

				// #angles - 6 letters
				std::getline(file, line);
				is.clear();
				is = std::istringstream(line.substr(7));
				is >> arr[0] >> arr[1] >> arr[2];

				tran.angles.x = arr[0];
				tran.angles.y = arr[1];
				tran.angles.z = arr[2];

				// #scale - 6  letters
				std::getline(file, line);
				is.clear();
				is = std::istringstream(line.substr(6));
				is >> arr[0] >> arr[1] >> arr[2];

				tran.scale.x = arr[0];
				tran.scale.y = arr[1];
				tran.scale.z = arr[2];

				// #general_scale - 12 letters
				std::getline(file, line);
				is.clear();
				is = std::istringstream(line.substr(12));
				is >> arr[0];

				tran.general_scale = arr[0];

				for (size_t i = 0; i < collection.getEntitiesCount(); i++) {
					const auto& entity = collection.getEntity(i);
					auto& transform = entity.getComponent<ecs::TransformComponent>();

					transform.center += (transform.center - tran.center);
					transform.angles += (transform.angles - tran.angles);
					transform.scale += (transform.scale - tran.scale);
					transform.general_scale += (transform.general_scale - tran.general_scale);

					transform.recalculate();
				}
			}
			else if (line.find("#EntityCollectionEnd") != std::string::npos) {
				return;
			}
		}
	}

	void Filesystem_Loading::loadEntity(std::ifstream& file, ecs::Scene* scene, const ecs::Entity& entity) {
		std::string line;

		while (std::getline(file, line)) {
			if (line.find("#TagComponent") != std::string::npos) {
				std::istringstream is(line.substr(14));
				std::string new_tag;
				is >> new_tag;
				auto& tag = entity.getComponent<ecs::TagComponent>();
				tag.tag = new_tag;
			}
			else if (line.find("#TransformComponent") != std::string::npos) {
				auto& tran = entity.getComponent<ecs::TransformComponent>();

				// #center - 7 letters
				std::getline(file, line);
				std::istringstream is(line.substr(7));
				float arr[3];
				is >> arr[0] >> arr[1] >> arr[2];

				tran.center.x = arr[0];
				tran.center.y = arr[1];
				tran.center.z = arr[2];

				// #angles - 6 letters
				std::getline(file, line);
				is.clear();
				is = std::istringstream(line.substr(7));
				is >> arr[0] >> arr[1] >> arr[2];

				tran.angles.x = arr[0];
				tran.angles.y = arr[1];
				tran.angles.z = arr[2];

				// #scale - 6  letters
				std::getline(file, line);
				is.clear();
				is = std::istringstream(line.substr(6));
				is >> arr[0] >> arr[1] >> arr[2];

				tran.scale.x = arr[0];
				tran.scale.y = arr[1];
				tran.scale.z = arr[2];

				// #general_scale - 12 letters
				std::getline(file, line);
				is.clear();
				is = std::istringstream(line.substr(12));
				is >> arr[0];

				tran.general_scale = arr[0];

				tran.recalculate();
			}
			else if (line.find("#RenderableComponent") != std::string::npos) {
				auto& ren = !entity.hasComponent<ecs::RenderableComponent>() ?
					entity.addComponent<ecs::RenderableComponent>(ECS_RENDERABLE) :
					entity.getComponent<ecs::RenderableComponent>();

				if (line.find("Cube") != std::string::npos) {
					ren.id = "Cube";
					ren.vertices = graphics::MeshCreator::Cube::getVertices();
					ren.indices = graphics::MeshCreator::Cube::getIndices();
				}
				else if (line.find("Surface") != std::string::npos) {
					ren.id = "Surface";
					ren.vertices = graphics::MeshCreator::Surface::getVertices();
					ren.indices = graphics::MeshCreator::Surface::getIndices();
				}
				else if (line.find("Wall") != std::string::npos) {
					ren.id = "Wall";
					ren.vertices = graphics::MeshCreator::Wall::getVertices();
					ren.indices = graphics::MeshCreator::Wall::getIndices();
				}
				else if (line.find("Pyramid") != std::string::npos) {
					ren.id = "Pyramid";
					ren.vertices = graphics::MeshCreator::Pyramid::getVertices();
					ren.indices = graphics::MeshCreator::Pyramid::getIndices();
				}
			}
			else if (line.find("#ColorComponent") != std::string::npos) {
				std::istringstream is(line.substr(16));
				float arr[3];
				is >> arr[0] >> arr[1] >> arr[2];

				auto& color = !entity.hasComponent<ecs::ColorComponent>() ?
					entity.addComponent<ecs::ColorComponent>(ECS_COLOR) :
					entity.getComponent<ecs::ColorComponent>();

				color.texture.x = arr[0];
				color.texture.y = arr[1];
				color.texture.z = arr[2];
			}
			else if (line.find("Texture2DComponent") != std::string::npos) {
				std::istringstream iss(line.substr(19));
				std::string tex;
				iss >> tex;

				auto& texture = !entity.hasComponent<ecs::Texture2DComponent>() ?
					entity.addComponent<ecs::Texture2DComponent>(ECS_TEXTURE2D) :
					entity.getComponent<ecs::Texture2DComponent>();

				texture.texture = tex;
			}
			else if (line.find("#TextureCubemapComponent") != std::string::npos) {
				std::istringstream iss(line.substr(25));
				std::string tex;
				iss >> tex;

				auto& cubemap = !entity.hasComponent<ecs::TextureCubemapComponent>() ?
					entity.addComponent<ecs::TextureCubemapComponent>(ECS_CUBEMAP) :
					entity.getComponent<ecs::TextureCubemapComponent>();

				cubemap.texture = tex;
			}
			else if (line.find("#LightComponent") != std::string::npos) {
				auto& light = !entity.hasComponent<ecs::LightComponent>() ?
					entity.addComponent<ecs::LightComponent>(ECS_LIGHT) :
					entity.getComponent<ecs::LightComponent>();

				// #ambientlight - 13
				std::getline(file, line);
				std::istringstream is(line.substr(13));
				float arr[3];
				is >> arr[0] >> arr[1] >> arr[2];

				light.ambient.x = arr[0];
				light.ambient.y = arr[1];
				light.ambient.z = arr[2];

				// #diffuselight - 13
				std::getline(file, line);
				is.clear();
				is = std::istringstream(line.substr(13));
				is >> arr[0] >> arr[1] >> arr[2];

				light.diffuse.x = arr[0];
				light.diffuse.y = arr[1];
				light.diffuse.z = arr[2];

				// #specularlight - 14
				std::getline(file, line);
				is.clear();
				is = std::istringstream(line.substr(14));
				is >> arr[0] >> arr[1] >> arr[2];

				light.specular.x = arr[0];
				light.specular.y = arr[1];
				light.specular.z = arr[2];

				// #constant - 9
				std::getline(file, line);
				is.clear();
				is = std::istringstream(line.substr(9));
				is >> arr[0];

				light.constant = arr[0];

				// #linear - 7
				std::getline(file, line);
				is.clear();
				is = std::istringstream(line.substr(7));
				is >> arr[0];

				light.linear = arr[0];

				// #quadratic - 10
				std::getline(file, line);
				is.clear();
				is = std::istringstream(line.substr(10));
				is >> arr[0];

				light.quadratic = arr[0];

				// #shininess - 10
				std::getline(file, line);
				is.clear();
				is = std::istringstream(line.substr(10));
				is >> arr[0];

				light.shininess = arr[0];
			}
			else if (line.find("#CameraComponent") != std::string::npos) {
				auto& cam = !entity.hasComponent<ecs::CameraComponent>() ?
					entity.addComponent<ecs::CameraComponent>(ECS_CAMERA) :
					entity.getComponent<ecs::CameraComponent>();

				std::string type;
				float var;

				// #id - 3
				std::getline(file, line);
				std::istringstream iss(line.substr(3));
				iss >> type;

				cam.id = type;

				// #used - 5
				std::getline(file, line);
				iss.clear();
				iss = std::istringstream(line.substr(5));
				type.clear();
				iss >> type;

				if (type.find("perspective") != std::string::npos) cam.Perspective = true;
				else cam.Perspective = false;

				// #type - 5
				std::getline(file, line);

				// #fov
				std::getline(file, line);
				iss.clear();
				iss = std::istringstream(line.substr(4));
				iss >> var;

				cam.p_fov = var;

				// #aspectratio
				std::getline(file, line);
				iss.clear();
				iss = std::istringstream(line.substr(12));
				iss >> var;

				cam.p_aspectRatio = var;

				// #near
				std::getline(file, line);
				iss.clear();
				iss = std::istringstream(line.substr(5));
				iss >> var;

				cam.p_near = var;

				// #far
				std::getline(file, line);
				iss.clear();
				iss = std::istringstream(line.substr(4));
				iss >> var;

				cam.p_far = var;

				// #type - 5
				std::getline(file, line);

				// #left
				std::getline(file, line);
				iss.clear();
				iss = std::istringstream(line.substr(5));
				iss >> var;

				cam.o_left = var;

				// #right
				std::getline(file, line);
				iss.clear();
				iss = std::istringstream(line.substr(6));
				iss >> var;

				cam.o_right = var;

				// #top
				std::getline(file, line);
				iss.clear();
				iss = std::istringstream(line.substr(4));
				iss >> var;

				cam.o_top = var;

				// #bottom
				std::getline(file, line);
				iss.clear();
				iss = std::istringstream(line.substr(7));
				iss >> var;

				cam.o_bottom = var;

				// #near
				std::getline(file, line);
				iss.clear();
				iss = std::istringstream(line.substr(5));
				iss >> var;

				cam.o_near = var;

				// #far
				std::getline(file, line);
				iss.clear();
				iss = std::istringstream(line.substr(4));
				iss >> var;

				cam.o_far = var;

				if (cam.id.find("main") != std::string::npos) {
					auto& renderCamera = scene->getRenderCamera();
					auto& tran = entity.getComponent<ecs::TransformComponent>();

					renderCamera.calculateCameraTransforms(tran, cam);
				}
			}
			else if (line.find("#ScriptComponent") != std::string::npos) {
				auto& script = !entity.hasComponent<ecs::ScriptComponent>() ?
					entity.addComponent<ecs::ScriptComponent>(ECS_SCRIPT) :
					entity.getComponent<ecs::ScriptComponent>();

				std::istringstream iss(line.substr(17));
				std::string s;
				iss >> s;
				script.script = s;
			}
			else if (line.find("#EntityEnd") != std::string::npos) {
				return;
			}
		}
	}


}
