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


	template<typename T>
	T& getComponentFromEntity(const ecs::Entity& entity, ecs::EntityComponents entcmp) {
		if (entity.hasComponent<T>()) { 
			return entity.getComponent<T>(); 
		}
		else {
			return entity.addComponent<T>(entcmp);
		}
	}

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

		auto loadVec3Getline = [&file, &line](maths::vec3& v, size_t num) {
			std::getline(file, line);
			std::istringstream is(line.substr(num));
			is >> v.x >> v.y >> v.z;
		};

		auto loadString = [&line](std::string& str, size_t num) {
			std::istringstream iss(line.substr(num));
			iss >> str;
		};

		auto loadFloatGetline = [&file, &line](float& f, size_t num) {
			std::getline(file, line);
			std::istringstream is(line.substr(num));
			is >> f;
		};

		while (std::getline(file, line)) {
			if (line.find("#EntityStart") != std::string::npos) {
				const auto& entity = collection.createEntity();
				loadEntity(file, scene, entity);
			}
			else if (line.find("#CollectionTagComponent") != std::string::npos) {
				auto& tag = collection.getComponent<ecs::TagComponent>();
				loadString(tag.tag, 24);
			}
			else if (line.find("#CollectionRenderableComponent") != std::string::npos) {
				auto& crc = collection.addComponent<ecs::CollectionRenderableComponent>();
				loadString(crc.id, 31);
				graphics::MeshCreator::loadOBJ("CannotFindOBJname", crc.id, collection);

				const auto size_after_loading = collection.getEntities().size();
				for (size_t i = 0; i < size_after_loading; ) {
					std::getline(file, line);
					if (line.find("#EntityStart") != std::string::npos) {
						const auto& entity = collection.getEntities()[i];
						loadEntity(file, scene, entity);
						i++;
					}
				}
			}
			else if (line.find("#CollectionTransformComponent") != std::string::npos) {
				auto& tran = collection.getComponent<ecs::TransformComponent>();

				// #center - 7 letters
				loadVec3Getline(tran.center, 7);

				// #angles - 7 letters
				loadVec3Getline(tran.angles, 7);

				// #scale - 6  letters
				loadVec3Getline(tran.scale, 6);

				// #general_scale - 12 letters
				loadFloatGetline(tran.general_scale, 12);
			}
			else if (line.find("#EntityCollectionEnd") != std::string::npos) {
				return;
			}
		}
	}

	void Filesystem_Loading::loadEntity(std::ifstream& file, ecs::Scene* scene, const ecs::Entity& entity) {
		std::string line;

		auto loadVec3 = [&line](maths::vec3& v, size_t num) {
			std::istringstream is(line.substr(num));
			is >> v.x >> v.y >> v.z;
		};

		auto loadVec3Getline = [&file, &line, &loadVec3](maths::vec3& v, size_t num) {
			std::getline(file, line);
			loadVec3(v, num);
		};

		auto loadString = [&line](std::string& str, size_t num) {
			std::istringstream iss(line.substr(num));
			iss >> str;
		};

		auto loadStringGetline = [&file, &line, &loadString](std::string& str, size_t num) {
			std::getline(file, line);
			loadString(str, num);
		};

		auto loadFloatGetline = [&file, &line](float& f, size_t num) {
			std::getline(file, line);
			std::istringstream is(line.substr(num));
			is >> f;
		};

		while (std::getline(file, line)) {
			if (line.find("#TagComponent") != std::string::npos) {
				auto& tag = entity.getComponent<ecs::TagComponent>();
				loadString(tag.tag, 14);
			}
			else if (line.find("#TransformComponent") != std::string::npos) {
				auto& tran = entity.getComponent<ecs::TransformComponent>();

				// #center - 7 letters
				loadVec3Getline(tran.center, 7);

				// #angles - 7 letters
				loadVec3Getline(tran.angles, 7);

				// #scale - 6  letters
				loadVec3Getline(tran.scale, 6);

				// #general_scale - 12 letters
				loadFloatGetline(tran.general_scale, 12);

				tran.recalculate();
			}
			else if (line.find("#RenderableComponent") != std::string::npos) {
				auto& ren = getComponentFromEntity<ecs::RenderableComponent>(entity, ECS_RENDERABLE);

				if (line.find("Cube") != std::string::npos) {
					ren.name = "Cube";
					ren.vertices = graphics::MeshCreator::Cube::getVertices();
					ren.indices = graphics::MeshCreator::Cube::getIndices();
				}
				else if (line.find("Surface") != std::string::npos) {
					ren.name = "Surface";
					ren.vertices = graphics::MeshCreator::Surface::getVertices();
					ren.indices = graphics::MeshCreator::Surface::getIndices();
				}
				else if (line.find("Wall") != std::string::npos) {
					ren.name = "Wall";
					ren.vertices = graphics::MeshCreator::Wall::getVertices();
					ren.indices = graphics::MeshCreator::Wall::getIndices();
				}
				else if (line.find("Pyramid") != std::string::npos) {
					ren.name = "Pyramid";
					ren.vertices = graphics::MeshCreator::Pyramid::getVertices();
					ren.indices = graphics::MeshCreator::Pyramid::getIndices();
				}
			}
			else if (line.find("#ColorComponent") != std::string::npos) {
				auto& color = getComponentFromEntity<ecs::ColorComponent>(entity, ECS_COLOR);
				loadVec3(color.texture, 16);
			}
			else if (line.find("Texture2DComponent") != std::string::npos) {
				auto& texture = getComponentFromEntity<ecs::Texture2DComponent>(entity, ECS_TEXTURE2D);
				loadString(texture.texture, 19);
			}
			else if (line.find("#TextureCubemapComponent") != std::string::npos) {
				auto& cubemap = getComponentFromEntity<ecs::TextureCubemapComponent>(entity, ECS_CUBEMAP);
				loadString(cubemap.texture, 25);
			}
			else if (line.find("#LightComponent") != std::string::npos) {
				auto& light = getComponentFromEntity<ecs::LightComponent>(entity, ECS_LIGHT);

				// #ambientlight - 13
				loadVec3Getline(light.ambient, 13);

				// #diffuselight - 13
				loadVec3Getline(light.diffuse, 13);

				// #specularlight - 14
				loadVec3Getline(light.specular, 14);

				// #constant - 9
				loadFloatGetline(light.constant, 9);

				// #linear - 7
				loadFloatGetline(light.linear, 7);

				// #quadratic - 10
				loadFloatGetline(light.quadratic, 10);

				// #shininess - 10
				loadFloatGetline(light.shininess, 10);
			}
			else if (line.find("#CameraComponent") != std::string::npos) {
				auto& cam = getComponentFromEntity<ecs::CameraComponent>(entity, ECS_CAMERA);

				// #id - 3
				loadStringGetline(cam.id, 3);

				// #used - 5
				std::string type;
				loadStringGetline(type, 5);

				if (type.find("perspective") != std::string::npos) { cam.Perspective = true; }
				else { cam.Perspective = false; }

				// #type - 5
				std::getline(file, line);

				// #fov - 4
				loadFloatGetline(cam.p_fov, 4);

				// #aspectratio - 12
				loadFloatGetline(cam.p_aspectRatio, 12);

				// #near - 5
				loadFloatGetline(cam.p_near, 5);

				// #far - 4
				loadFloatGetline(cam.p_far, 4);

				// #type - 5
				std::getline(file, line);

				// #left - 5
				loadFloatGetline(cam.o_left, 5);

				// #right - 6
				loadFloatGetline(cam.o_right, 6);

				// #top - 4
				loadFloatGetline(cam.o_top, 4);

				// #bottom - 7
				loadFloatGetline(cam.o_bottom, 7);

				// #near - 5
				loadFloatGetline(cam.o_near, 5);

				// #far
				loadFloatGetline(cam.o_far, 4);

				if (cam.id.find("main") != std::string::npos) {
					auto& renderCamera = scene->getRenderCamera();
					const auto& tran = entity.getComponent<ecs::TransformComponent>();

					renderCamera.calculateCameraTransforms(tran, cam);
				}
			}
			else if (line.find("#ScriptComponent") != std::string::npos) {
				auto& script = getComponentFromEntity<ecs::ScriptComponent>(entity, ECS_SCRIPT);
				loadString(script.script, 17);
			}
			else if (line.find("#EntityEnd") != std::string::npos) {
				return;
			}
		}
	}


}
