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
#include "../ecs/Components/Components.h"


namespace marengine {


	Scene* FSceneDeserializer::loadSceneFromFile(const std::string& path) {
		const std::string extension{ path.substr(path.find_last_of(".") + 1) };
		if (extension != "json") {
			FILESYSTEM_ERROR("FILESYSTEM: path extension is not json! Found {} in given path {}", extension, path);
			return Scene::createEmptyScene("EmptySceneNotLoaded");
		}

		nlohmann::json json;

		std::ifstream file(path);
		if (!file.is_open()) {
			FILESYSTEM_ERROR("FILESYSTEM: cannot open file at given path {}!", path);
			return Scene::createEmptyScene("EmptySceneNotLoaded");
		}
		file >> json;
		file.close();

		const std::string sceneName{ json["Scene"]["name"].get<std::string>() };
		const float backX{ json["Scene"][sceneName]["Background"]["x"].get<float>() };
		const float backY{ json["Scene"][sceneName]["Background"]["y"].get<float>() };
		const float backZ{ json["Scene"][sceneName]["Background"]["z"].get<float>() };
		
		Scene* scene{ new Scene("Default") };
		scene->setName(sceneName);
		scene->setBackground({ backX, backY, backZ });

		const uint32_t entitiesCount{ json["Scene"][sceneName]["Info"]["EntitiesCount"].get<uint32_t>() };

		for (uint32_t i = 0; i < entitiesCount; i++) {
			const std::string entityTag{ json["Scene"][sceneName]["Info"]["Entities"][std::to_string(i)]["tag"].get<std::string>() };
			const uint32_t childsCount{ json["Scene"][sceneName]["Info"]["Entities"][std::to_string(i)]["childsCount"].get<uint32_t>() };
		
			const Entity& entity{ scene->createEntity() };
			loadEntity(entity, json, entityTag, childsCount, sceneName);
		}

		return scene;
	}

	void FSceneDeserializer::loadEntity(const Entity& entity, nlohmann::json& json, const std::string& entityTag, uint32_t childsCount, const std::string& sceneName) {
		auto jsonContains = [&sceneName, &entityTag, &json](const char* componentName)->bool {
			return json["Scene"][sceneName]["Entity"][entityTag].contains(componentName);
		};
		auto setString = [&sceneName, &entityTag, &json](std::string& str, const char* componentName, const char* value) {
			str = json["Scene"][sceneName]["Entity"][entityTag][componentName][value];
		};
		auto loadFloat = [&sceneName, &entityTag, &json](const char* componentName, const char* value)->float {
			return json["Scene"][sceneName]["Entity"][entityTag][componentName][value].get<float>();
		};
		auto loadVec3 = [&sceneName, &entityTag, &json](const char* componentName, const char* value)->maths::vec4 {
			return {
				json["Scene"][sceneName]["Entity"][entityTag][componentName][value]["x"].get<float>(),
				json["Scene"][sceneName]["Entity"][entityTag][componentName][value]["y"].get<float>(),
				json["Scene"][sceneName]["Entity"][entityTag][componentName][value]["z"].get<float>(),
				0.f 
			};
		};

		auto& tag{ entity.getComponent<TagComponent>().tag };
		setString(tag, "TagComponent", "tag");

		auto& transform{ entity.getComponent<TransformComponent>() };
		transform.center = loadVec3("TransformComponent", "center");
		transform.angles = loadVec3("TransformComponent", "angles");
		transform.scale = loadVec3("TransformComponent", "scale");

		if (jsonContains("RenderableComponent")) {
			auto& renderable{ entity.addComponent<RenderableComponent>() };
			setString(renderable.name, "RenderableComponent", "name");
		}

		if (jsonContains("ColorComponent")) {
			auto& color{ entity.addComponent<ColorComponent>() };
			color.texture = loadVec3("ColorComponent", "texture");
		}
		else if (jsonContains("Texture2DComponent")) {
			auto& tex{ entity.addComponent<Texture2DComponent>() };
			setString(tex.texture, "Texture2DComponent", "name");
		}
		else if (jsonContains("TextureCubemapComponent")) {
			auto& cube{ entity.addComponent<TextureCubemapComponent>() };
			setString(cube.texture, "TextureCubemapComponent", "name");
		}

		if (jsonContains("LightComponent")) {
			auto& light{ entity.addComponent<LightComponent>() };
			light.ambient = loadVec3("LightComponent", "ambient");
			light.diffuse = loadVec3("LightComponent", "diffuse");
			light.specular = loadVec3("LightComponent", "specular");
			light.constant = loadFloat("LightComponent", "constant");
			light.linear = loadFloat("LightComponent", "linear");
			light.quadratic = loadFloat("LightComponent", "quadratic");
			light.shininess = loadFloat("LightComponent", "shininess");
		}

		if (jsonContains("CameraComponent")) {
			auto& cam{ entity.addComponent<CameraComponent>() };
			setString(cam.id, "CameraComponent", "id");
			cam.Perspective = loadFloat("CameraComponent", "Perspective") == 1.0f ? true : false;

			// Perspective parameters loading
			cam.p_fov = loadFloat("CameraComponent", "p_fov");
			cam.p_aspectRatio = loadFloat("CameraComponent", "p_aspectRatio");
			cam.p_near = loadFloat("CameraComponent", "p_near");
			cam.p_far = loadFloat("CameraComponent", "p_far");

			// Orthographic parameters loading
			cam.o_left = loadFloat("CameraComponent", "o_left");
			cam.o_right = loadFloat("CameraComponent", "o_right");
			cam.o_bottom = loadFloat("CameraComponent", "o_bottom");
			cam.o_near = loadFloat("CameraComponent", "o_near");
			cam.o_far = loadFloat("CameraComponent", "o_far");
		}

		if (jsonContains("ScriptComponent")) {
			auto& script{ entity.addComponent<ScriptComponent>() };
			setString(script.script, "ScriptComponent", "path");
		}
	}


}