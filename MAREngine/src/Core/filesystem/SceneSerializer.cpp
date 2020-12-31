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


#include "SceneSerializer.h"
#include "../ecs/Scene.h"
#include "../ecs/Entity/Entity.h"
#include "../ecs/Components/Components.h"


namespace marengine {


	void FSceneSerializer::saveSceneToFile(const char* path, Scene* scene) {
		nlohmann::json json;

		json["Documentation"]["Engine"] = "MAREngine";
		json["Documentation"]["EngineVersion"] = "v0.0.1";
		json["Documentation"]["App"] = "DefaultApplication";
		json["Documentation"]["AppVersion"] = "v1.0.0";

		const std::string& sceneName{ scene->getName() };
		const maths::vec3& background{ scene->getBackground() };

		json["Scene"]["Name"] = sceneName;
		json["Scene"][sceneName]["Background"]["x"] = background.x;
		json["Scene"][sceneName]["Background"]["y"] = background.y;
		json["Scene"][sceneName]["Background"]["z"] = background.z;

		const std::vector<Entity>& entities{ scene->getEntities() };
		const uint32_t entitiesSize{ entities.size() };

		json["Scene"][sceneName]["Info"]["EntitiesCount"] = entitiesSize;
		
		for (uint32_t i = 0; i < entitiesSize; i++) {
			const auto& tag{ entities[i].getComponent<TagComponent>().tag };
			json["Scene"][sceneName]["Info"]["Entities"][i]["tag"] = tag;
			json["Scene"][sceneName]["Info"]["Entities"][i]["childsCount"] = entities[i].getChilds().size();

			saveEntity(entities[i], json, sceneName);
		}
		
		std::ofstream ss(path, std::ios::out | std::ios::trunc);
		ss << json.dump(4);
		ss.close();
	}

	void FSceneSerializer::saveEntity(const Entity& entity, nlohmann::json& json, const std::string& sceneName) {
		const std::string& tag{ entity.getComponent<TagComponent>() };
		auto saveString = [&json, &tag, &sceneName](const char* componentName, const char* value, const std::string& str) {
			json["Scene"][sceneName]["Entity"][tag][componentName][value] = str;
		};
		auto saveFloat = [&json, &tag, &sceneName](const char* componentName, const char* value, float f) {
			json["Scene"][sceneName]["Entity"][tag][componentName][value] = f;
		};
		auto saveVec3 = [&json, &tag, &sceneName](const char* componentName, const char* value, const maths::vec3& v) {
			json["Scene"][sceneName]["Entity"][tag][componentName][value]["x"] = v.x;
			json["Scene"][sceneName]["Entity"][tag][componentName][value]["y"] = v.y;
			json["Scene"][sceneName]["Entity"][tag][componentName][value]["z"] = v.z;
		};

		saveString("TagComponent", "tag", tag);

		// Save transformComponent
		const auto& transform{ entity.getComponent<TransformComponent>() };

		saveVec3("TransformComponent", "center", transform.center);
		saveVec3("TransformComponent", "angles", transform.angles);
		saveVec3("TransformComponent", "scale", transform.scale);

		if (entity.hasComponent<RenderableComponent>()) {
			const auto& renderable{ entity.getComponent<RenderableComponent>() };
			saveString("RenderableComponent", "name", renderable.name);
		}

		if (entity.hasComponent<ColorComponent>()) {
			const auto& color{ entity.getComponent<ColorComponent>() };
			saveVec3("ColorComponent", "texture", color.texture);
		}
		else if (entity.hasComponent<Texture2DComponent>()) {
			const auto& tex{ entity.getComponent<Texture2DComponent>() };
			saveString("Texture2DComponent", "name", tex.texture);
		}
		else if (entity.hasComponent<TextureCubemapComponent>()) {
			const auto& cube{ entity.getComponent<TextureCubemapComponent>() };
			saveString("TextureCubemapComponent", "name", cube.texture);
		}
		
		if (entity.hasComponent<LightComponent>()) {
			const auto& light{ entity.getComponent<LightComponent>() };
			saveVec3("LightComponent", "ambient", light.ambient);
			saveVec3("LightComponent", "diffuse", light.diffuse);
			saveVec3("LightComponent", "specular", light.specular);
			saveFloat("LightComponent", "constant", light.constant);
			saveFloat("LightComponent", "linear", light.linear);
			saveFloat("LightComponent", "quadratic", light.quadratic);
			saveFloat("LightComponent", "shininess", light.shininess);
		}

		if (entity.hasComponent<CameraComponent>()) {
			const auto& cam{ entity.getComponent<CameraComponent>() };
			saveString("CameraComponent", "id", cam.id);
			saveFloat("CameraComponent", "Perspective", cam.Perspective ? 1.f : 0.f);

			// Perspective camera save
			saveFloat("CameraComponent", "p_fov", cam.p_fov);
			saveFloat("CameraComponent", "p_aspectRatio", cam.p_aspectRatio);
			saveFloat("CameraComponent", "p_near", cam.p_near);
			saveFloat("CameraComponent", "p_far", cam.p_far);

			// Orthographic camera save
			saveFloat("CameraComponent", "o_left", cam.o_left);
			saveFloat("CameraComponent", "o_right", cam.o_right);
			saveFloat("CameraComponent", "o_top", cam.o_top);
			saveFloat("CameraComponent", "o_bottom", cam.o_bottom);
			saveFloat("CameraComponent", "o_near", cam.o_near);
			saveFloat("CameraComponent", "o_far", cam.o_far);
		}

		if (entity.hasComponent<ScriptComponent>()) {
			const auto& script{ entity.getComponent<ScriptComponent>() };
			saveString("ScriptComponent", "path", script.script);
		}
	}


}
