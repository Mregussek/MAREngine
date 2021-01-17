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
#include "FilesystemLogging.h"
#include "../ecs/Scene.h"
#include "../ecs/Entity/Entity.h"
#include "../ecs/Components/Components.h"


namespace marengine {


	void FSceneSerializer::saveSceneToFile(const char* path, Scene* scene) {
		std::ofstream ss(path, std::ios::out | std::ios::trunc);
		if (!ss.is_open()) {
			FILESYSTEM_ERROR("FILESSYTEM: Cannot open file {} and save scene at {}!", name, filename);
			return;
		}

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

		for (uint32_t i = 0; i < entitiesSize; i++) {
			const auto& tag{ entities[i].getComponent<TagComponent>().tag };

			saveEntity(entities[i], i, json, sceneName);
		}
		
		ss << json.dump(4);
		ss.close();
	}

	void FSceneSerializer::saveEntity(const Entity& entity, uint32_t index, nlohmann::json& json, const std::string& sceneName) {
		
		auto saveString = [&json, &sceneName, index](const char* componentName, const char* value, const std::string& str) {
			json["Scene"][sceneName]["Entity"][index][componentName][value] = str;
		};
		auto saveFloat = [&json, &sceneName, index](const char* componentName, const char* value, float f) {
			json["Scene"][sceneName]["Entity"][index][componentName][value] = f;
		};
		auto saveVec3 = [&json, &sceneName, index](const char* componentName, const char* value, const maths::vec3& v) {
			json["Scene"][sceneName]["Entity"][index][componentName][value]["x"] = v.x;
			json["Scene"][sceneName]["Entity"][index][componentName][value]["y"] = v.y;
			json["Scene"][sceneName]["Entity"][index][componentName][value]["z"] = v.z;
		};
		auto saveVec4 = [&json, &sceneName, index](const char* componentName, const char* value, const maths::vec4& v) {
			json["Scene"][sceneName]["Entity"][index][componentName][value]["x"] = v.x;
			json["Scene"][sceneName]["Entity"][index][componentName][value]["y"] = v.y;
			json["Scene"][sceneName]["Entity"][index][componentName][value]["z"] = v.z;
			json["Scene"][sceneName]["Entity"][index][componentName][value]["w"] = v.w;
		};

		const auto& tagComponent{ entity.getComponent<TagComponent>() };
		saveString("TagComponent", "tag", tagComponent.tag);

		const auto& transformComponent{ entity.getComponent<TransformComponent>() };
		saveVec3("TransformComponent", "position", transformComponent.position);
		saveVec3("TransformComponent", "rotation", transformComponent.rotation);
		saveVec3("TransformComponent", "scale", transformComponent.scale);

		if (entity.hasComponent<RenderableComponent>()) {
			const auto& renderableComponent{ entity.getComponent<RenderableComponent>() };
			saveString("RenderableComponent", "name", renderableComponent.name);
		}

		if (entity.hasComponent<ColorComponent>()) {
			const auto& colorComponent{ entity.getComponent<ColorComponent>() };
			saveVec4("ColorComponent", "color", colorComponent.color);
		}
		else if (entity.hasComponent<Texture2DComponent>()) {
			const auto& texture2DComponent{ entity.getComponent<Texture2DComponent>() };
			saveString("Texture2DComponent", "texturePath", texture2DComponent.texturePath);
		}
		else if (entity.hasComponent<TextureCubemapComponent>()) {
			const auto& cubemapComponent{ entity.getComponent<TextureCubemapComponent>() };
			saveString("TextureCubemapComponent", "texturePath", cubemapComponent.texturePath);
		}
		
		if (entity.hasComponent<PointLightComponent>()) {
			const auto& pointLightComponent{ entity.getComponent<PointLightComponent>() };
			
			saveVec4("PointLightComponent", "position", pointLightComponent.pointLight.position);
			saveVec4("PointLightComponent", "ambient", pointLightComponent.pointLight.ambient);
			saveVec4("PointLightComponent", "diffuse", pointLightComponent.pointLight.diffuse);
			saveVec4("PointLightComponent", "specular", pointLightComponent.pointLight.specular);
			saveFloat("PointLightComponent", "constant", pointLightComponent.pointLight.constant);
			saveFloat("PointLightComponent", "linear", pointLightComponent.pointLight.linear);
			saveFloat("PointLightComponent", "quadratic", pointLightComponent.pointLight.quadratic);
			saveFloat("PointLightComponent", "shininess", pointLightComponent.pointLight.shininess);
		}

		if (entity.hasComponent<CameraComponent>()) {
			const auto& cameraComponent{ entity.getComponent<CameraComponent>() };
			saveString("CameraComponent", "id", cameraComponent.id);
			saveFloat("CameraComponent", "Perspective", cameraComponent.Perspective ? 1.f : 0.f);

			// Perspective camera save
			saveFloat("CameraComponent", "p_fov", cameraComponent.p_fov);
			saveFloat("CameraComponent", "p_aspectRatio", cameraComponent.p_aspectRatio);
			saveFloat("CameraComponent", "p_near", cameraComponent.p_near);
			saveFloat("CameraComponent", "p_far", cameraComponent.p_far);

			// Orthographic camera save
			saveFloat("CameraComponent", "o_left", cameraComponent.o_left);
			saveFloat("CameraComponent", "o_right", cameraComponent.o_right);
			saveFloat("CameraComponent", "o_top", cameraComponent.o_top);
			saveFloat("CameraComponent", "o_bottom", cameraComponent.o_bottom);
			saveFloat("CameraComponent", "o_near", cameraComponent.o_near);
			saveFloat("CameraComponent", "o_far", cameraComponent.o_far);
		}

		if (entity.hasComponent<PythonScriptComponent>()) {
			const auto& pythonScriptComponent{ entity.getComponent<PythonScriptComponent>() };
			saveString("PythonScriptComponent", "path", pythonScriptComponent.scriptsPath);
		}
	}


}
