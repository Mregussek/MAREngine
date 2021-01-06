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


#include "SceneDeserializer.h"
#include "FilesystemLogging.h"
#include "../ecs/Scene.h"
#include "../ecs/Entity/Entity.h"
#include "../graphics/Mesh/MeshCreator.h"
#include "../ecs/Components/Components.h"


namespace marengine {


	Scene* FSceneDeserializer::loadSceneFromFile(const std::string& path) {
		const std::string extension{ path.substr(path.find_last_of(".") + 1) };
		if (extension != "json") {
			FILESYSTEM_ERROR("FILESYSTEM: path extension is not json! Found {} in given path {}", extension, path);
			return Scene::createEmptyScene("EmptySceneNotLoaded");
		}

		
		std::ifstream file(path);
		if (!file.is_open()) {
			FILESYSTEM_ERROR("FILESYSTEM: cannot open file at given path {}!", path);
			return Scene::createEmptyScene("EmptySceneNotLoaded");
		}
		nlohmann::json json{ nlohmann::json::parse(file) };
		file.close();

		const std::string sceneName{ json["Scene"]["Name"] };
		const float backX{ json["Scene"][sceneName]["Background"]["x"].get<float>() };
		const float backY{ json["Scene"][sceneName]["Background"]["y"].get<float>() };
		const float backZ{ json["Scene"][sceneName]["Background"]["z"].get<float>() };
		
		Scene* scene{ new Scene("Default") };
		scene->setName(sceneName);
		scene->setBackground({ backX, backY, backZ });

		uint32_t i = 0;
		for (nlohmann::json& jsonEntity : json["Scene"][sceneName]["Entity"]) {
			const Entity& entity{ scene->createEntity() };
			loadEntity(entity, i, json, sceneName);
			i++;
		}

		return scene;
	}

	void fillRenderable(RenderableComponent& renderable) {
		if (renderable.name.find("Cube") != std::string::npos) {
			renderable.vertices = MeshCreator::Cube::getVertices();
			renderable.indices = MeshCreator::Cube::getIndices();
		}
		else if (renderable.name.find("Surface") != std::string::npos) {
			renderable.vertices = MeshCreator::Surface::getVertices();
			renderable.indices = MeshCreator::Surface::getIndices();
		}
		else if (renderable.name.find("Wall") != std::string::npos) {
			renderable.vertices = MeshCreator::Wall::getVertices();
			renderable.indices = MeshCreator::Wall::getIndices();
		}
		else if (renderable.name.find("Pyramid") != std::string::npos) {
			renderable.vertices = MeshCreator::Pyramid::getVertices();
			renderable.indices = MeshCreator::Pyramid::getIndices();
		}
	}

	void FSceneDeserializer::loadEntity(const Entity& entity, uint32_t index, nlohmann::json& json, const std::string& sceneName) {
		auto jsonContains = [&sceneName, index, &json](const char* componentName)->bool {
			return json["Scene"][sceneName]["Entity"][index].contains(componentName);
		};
		auto setString = [&sceneName, index, &json](std::string& str, const char* componentName, const char* value) {
			str = json["Scene"][sceneName]["Entity"][index][componentName][value];
		};
		auto loadFloat = [&sceneName, index, &json](const char* componentName, const char* value)->float {
			return json["Scene"][sceneName]["Entity"][index][componentName][value].get<float>();
		};
		auto loadVec3 = [&sceneName, index, &json](const char* componentName, const char* value)->maths::vec4 {
			return {
				json["Scene"][sceneName]["Entity"][index][componentName][value]["x"].get<float>(),
				json["Scene"][sceneName]["Entity"][index][componentName][value]["y"].get<float>(),
				json["Scene"][sceneName]["Entity"][index][componentName][value]["z"].get<float>(),
				0.f 
			};
		};

		auto& tagComponent{ entity.getComponent<TagComponent>() };
		setString(tagComponent.tag, "TagComponent", "tag");

		auto& transformComponent{ entity.getComponent<TransformComponent>() };
		transformComponent.position = loadVec3("TransformComponent", "center");
		transformComponent.rotation = loadVec3("TransformComponent", "angles");
		transformComponent.scale = loadVec3("TransformComponent", "scale");

		if (jsonContains("RenderableComponent")) {
			auto& renderableComponent{ entity.addComponent<RenderableComponent>() };
			setString(renderableComponent.name, "RenderableComponent", "name");
			fillRenderable(renderableComponent);
		}

		if (jsonContains("ColorComponent")) {
			auto& colorComponent{ entity.addComponent<ColorComponent>() };
			colorComponent.color = loadVec3("ColorComponent", "texture");
		}

		if (jsonContains("Texture2DComponent")) {
			auto& texture2DComponent{ entity.addComponent<Texture2DComponent>() };
			setString(texture2DComponent.texturePath, "Texture2DComponent", "name");
		}

		if (jsonContains("TextureCubemapComponent")) {
			auto& cubemapComponent{ entity.addComponent<TextureCubemapComponent>() };
			setString(cubemapComponent.texturePath, "TextureCubemapComponent", "name");
		}

		if (jsonContains("PointLightComponent")) {
			auto& pointLightComponent{ entity.addComponent<PointLightComponent>() };
			pointLightComponent.pointLight.ambient = loadVec3("PointLightComponent", "ambient");
			pointLightComponent.pointLight.diffuse = loadVec3("PointLightComponent", "diffuse");
			pointLightComponent.pointLight.specular = loadVec3("PointLightComponent", "specular");
			pointLightComponent.pointLight.constant = loadFloat("PointLightComponent", "constant");
			pointLightComponent.pointLight.linear = loadFloat("PointLightComponent", "linear");
			pointLightComponent.pointLight.quadratic = loadFloat("PointLightComponent", "quadratic");
			pointLightComponent.pointLight.shininess = loadFloat("PointLightComponent", "shininess");
		}

		if (jsonContains("CameraComponent")) {
			auto& cameraComponent{ entity.addComponent<CameraComponent>() };
			setString(cameraComponent.id, "CameraComponent", "id");
			cameraComponent.Perspective = loadFloat("CameraComponent", "Perspective") == 1.0f ? true : false;

			// Perspective parameters loading
			cameraComponent.p_fov = loadFloat("CameraComponent", "p_fov");
			cameraComponent.p_aspectRatio = loadFloat("CameraComponent", "p_aspectRatio");
			cameraComponent.p_near = loadFloat("CameraComponent", "p_near");
			cameraComponent.p_far = loadFloat("CameraComponent", "p_far");

			// Orthographic parameters loading
			cameraComponent.o_left = loadFloat("CameraComponent", "o_left");
			cameraComponent.o_right = loadFloat("CameraComponent", "o_right");
			cameraComponent.o_bottom = loadFloat("CameraComponent", "o_bottom");
			cameraComponent.o_near = loadFloat("CameraComponent", "o_near");
			cameraComponent.o_far = loadFloat("CameraComponent", "o_far");
		}

		if (jsonContains("PythonScriptComponent")) {
			auto& pythonScriptComponent{ entity.addComponent<PythonScriptComponent>() };
			setString(pythonScriptComponent.scriptsPath, "PythonScriptComponent", "path");
		}
	}

	Scene* FSceneDeserializer::oldWayLoadingFile(const std::string& filename) {
		FILESYSTEM_INFO("FILESYSTEM: going to load scene from: {}", filename);

		std::ifstream file(filename);

		if (!file.is_open()) {
			FILESYSTEM_ERROR("FILESYSTEM: cannot open file, returning empty scene...")
			return Scene::createEmptyScene("EmptySceneNotLoaded");
		}

		std::string line;
		Scene* scene{ nullptr };

		while (std::getline(file, line)) {
			if (line.find("#SceneStart") != std::string::npos) {
				std::getline(file, line);
				std::istringstream is(line.substr(11));
				std::string name;
				is >> name;
				scene = new Scene(name);

				std::getline(file, line);
				maths::vec3 background;
				is.clear();
				is = std::istringstream(line.substr(18));
				is >> background.x >> background.y >> background.z;
				scene->setBackground(background);

				loadScene(file, scene);
			}
		}

		file.close();

		FILESYSTEM_ERROR("FILESYSTEM: returning loaded scene {} from file {}", scene->getName(), filename);

		return scene;
	}

	void FSceneDeserializer::loadScene(std::ifstream& file, Scene* scene) {
		std::string line;

		while (std::getline(file, line)) {
			if (line.find("#EntityStart") != std::string::npos) {
				const auto& entity = scene->createEntity();
				loadEntity(file, scene, entity);
			}
			else if (line.find("#SceneEnd") != std::string::npos) {
				return;
			}
		}
	}

	void FSceneDeserializer::loadEntity(std::ifstream& file, Scene* scene, const Entity& entity) {
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

				loadVec3Getline(tran.position, 7); // #center - 7 letters
				loadVec3Getline(tran.rotation, 7); // #angles - 7 letters
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
				loadVec4(color.color, 16);
			}
			else if (line.find("Texture2DComponent") != std::string::npos) {
				auto& texture{ entity.get_addComponent<Texture2DComponent>() };
				loadString(texture.texturePath, 19);
			}
			else if (line.find("#TextureCubemapComponent") != std::string::npos) {
				auto& cubemap{ entity.get_addComponent<TextureCubemapComponent>() };
				loadString(cubemap.texturePath, 25);
			}
			else if (line.find("#PointLightComponent") != std::string::npos) {
				auto& light{ entity.get_addComponent<PointLightComponent>() };

				loadVec4Getline(light.pointLight.ambient, 13);		// #ambientlight - 13
				loadVec4Getline(light.pointLight.diffuse, 13);		// #diffuselight - 13
				loadVec4Getline(light.pointLight.specular, 14);		// #specularlight - 14

				loadFloatGetline(light.pointLight.constant, 9);		// #constant - 9
				loadFloatGetline(light.pointLight.linear, 7);		// #linear - 7
				loadFloatGetline(light.pointLight.quadratic, 10);	// #quadratic - 10
				loadFloatGetline(light.pointLight.shininess, 10);	// #shininess - 10
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
			else if (line.find("#PythonScriptComponent") != std::string::npos) {
				auto& script{ entity.get_addComponent<PythonScriptComponent>() };
				loadString(script.scriptsPath, 23);
			}
			else if (line.find("#EntityEnd") != std::string::npos) {
				return;
			}
		}
	}


}