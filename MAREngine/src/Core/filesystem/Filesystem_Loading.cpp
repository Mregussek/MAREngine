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
#include "../../Core/ecs/SceneOptimizer.h"
#include "../../Core/ecs/Entity/Entity.h"
#include "../../Core/ecs/Entity/EntityCollection.h"
#include "../../Core/ecs/Components/Components.h"


namespace marengine {


	void Filesystem_Loading::loadScene(std::ifstream& file, Scene* scene) {
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

	void Filesystem_Loading::loadCollection(std::ifstream& file, Scene* scene, const EntityCollection& collection) {
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
				auto& tag = collection.getComponent<TagComponent>();
				loadString(tag.tag, 24);
			}
			else if (line.find("#CollectionRenderableComponent") != std::string::npos) {
				std::string path{};
				loadString(path, 31);
				const auto filename{ path.substr(path.find_last_of("/") + 1, path.size()) };

				const auto collectionIndex{ SceneOptimizer::checkIfOBJhasBeenLoaded(path, scene) };
				if (collectionIndex != -1) {
					SceneOptimizer::copyOBJtoOtherCollectionDuringSceneLoading(collection, scene->getCollections()[collectionIndex]);
				}
				else {
					MeshCreator::loadOBJ(filename, path, collection);
				}

				const auto sizeAfterLoading{ collection.getEntities().size() };
				for (size_t i = 0; i < sizeAfterLoading; ) {
					std::getline(file, line);
					if (line.find("#EntityStart") != std::string::npos) {
						loadEntity(file, scene, collection.getEntities()[i]);
						i++;
					}
				}
			}
			else if (line.find("#CollectionTransformComponent") != std::string::npos) {
				auto& tran = collection.getComponent<TransformComponent>();

				loadVec3Getline(tran.center, 7); // #center - 7 letters
				loadVec3Getline(tran.angles, 7); // #angles - 7 letters
				loadVec3Getline(tran.scale, 6);  // #scale  - 6  letters
			}
			else if (line.find("#EntityCollectionEnd") != std::string::npos) {
				return;
			}
		}
	}

	void Filesystem_Loading::loadEntity(std::ifstream& file, Scene* scene, const Entity& entity) {
		std::string line;

		auto loadVec3 = [&line](maths::vec3& v, size_t num) {
			std::istringstream is(line.substr(num));
			is >> v.x >> v.y >> v.z;
		};

		auto loadVec3Getline = [&file, &line, &loadVec3](maths::vec3& v, size_t num) {
			std::getline(file, line);
			loadVec3(v, num);
		};

		auto loadVec4 = [&line](maths::vec4& v, size_t num) {
			std::istringstream is(line.substr(num));
			is >> v.x >> v.y >> v.z >> v.w;
		};

		auto loadVec4Getline = [&file, &line, &loadVec4](maths::vec4& v, size_t num) {
			std::getline(file, line);
			loadVec4(v, num);
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
				auto& tag = entity.getComponent<TagComponent>();
				loadString(tag.tag, 14);
			}
			else if (line.find("#TransformComponent") != std::string::npos) {
				auto& tran = entity.getComponent<TransformComponent>();

				loadVec3Getline(tran.center, 7); // #center - 7 letters
				loadVec3Getline(tran.angles, 7); // #angles - 7 letters
				loadVec3Getline(tran.scale, 6);  // #scale  - 6  letters
			}
			else if (line.find("#RenderableComponent") != std::string::npos) {
				auto& ren{ entity.get_addComponent<RenderableComponent>() };

				if (line.find("Cube") != std::string::npos) {
					ren.name = "Cube";
					ren.vertices = MeshCreator::Cube::getVertices();
					ren.indices = MeshCreator::Cube::getIndices();
				}
				else if (line.find("Surface") != std::string::npos) {
					ren.name = "Surface";
					ren.vertices = MeshCreator::Surface::getVertices();
					ren.indices = MeshCreator::Surface::getIndices();
				}
				else if (line.find("Wall") != std::string::npos) {
					ren.name = "Wall";
					ren.vertices = MeshCreator::Wall::getVertices();
					ren.indices = MeshCreator::Wall::getIndices();
				}
				else if (line.find("Pyramid") != std::string::npos) {
					ren.name = "Pyramid";
					ren.vertices = MeshCreator::Pyramid::getVertices();
					ren.indices = MeshCreator::Pyramid::getIndices();
				}
			}
			else if (line.find("#ColorComponent") != std::string::npos) {
				auto& color{ entity.get_addComponent<ColorComponent>() };
				loadVec4(color.texture, 16);
			}
			else if (line.find("Texture2DComponent") != std::string::npos) {
				auto& texture{ entity.get_addComponent<Texture2DComponent>() };
				loadString(texture.texture, 19);
			}
			else if (line.find("#TextureCubemapComponent") != std::string::npos) {
				auto& cubemap{ entity.get_addComponent<TextureCubemapComponent>() };
				loadString(cubemap.texture, 25);
			}
			else if (line.find("#LightComponent") != std::string::npos) {
				auto& light{ entity.get_addComponent<LightComponent>() };

				loadVec4Getline(light.ambient, 13);		// #ambientlight - 13
				loadVec4Getline(light.diffuse, 13);		// #diffuselight - 13
				loadVec4Getline(light.specular, 14);	// #specularlight - 14

				loadFloatGetline(light.constant, 9);	// #constant - 9
				loadFloatGetline(light.linear, 7);		// #linear - 7
				loadFloatGetline(light.quadratic, 10);	// #quadratic - 10
				loadFloatGetline(light.shininess, 10);	// #shininess - 10
			}
			else if (line.find("#CameraComponent") != std::string::npos) {
				auto& cam{ entity.get_addComponent<CameraComponent>() };
				std::string type;
				
				loadStringGetline(cam.id, 3);	// #id - 3
				loadStringGetline(type, 5);		// #used - 5

				if (type.find("perspective") != std::string::npos) { cam.Perspective = true; }
				else { cam.Perspective = false; }

				std::getline(file, line); // #type - 5	perspective

				loadFloatGetline(cam.p_fov, 4);				// #fov - 4
				loadFloatGetline(cam.p_aspectRatio, 12);	// #aspectratio - 12
				loadFloatGetline(cam.p_near, 5);			// #near - 5
				loadFloatGetline(cam.p_far, 4);				// #far - 4

				std::getline(file, line); // #type - 5 orthographic

				loadFloatGetline(cam.o_left, 5);	// #left - 5
				loadFloatGetline(cam.o_right, 6);	// #right - 6
				loadFloatGetline(cam.o_top, 4);		// #top - 4
				loadFloatGetline(cam.o_bottom, 7);	// #bottom - 7
				loadFloatGetline(cam.o_near, 5);	// #near - 5
				loadFloatGetline(cam.o_far, 4);		// #far
			}
			else if (line.find("#ScriptComponent") != std::string::npos) {
				auto& script{ entity.get_addComponent<ScriptComponent>() };
				loadString(script.script, 17);
			}
			else if (line.find("#EntityEnd") != std::string::npos) {
				return;
			}
		}
	}


}
