/***********************************************************************
* @internal @copyright
*
*  				MAREngine - open source 3D game engine
*
* Copyright (C) 2020-present Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
* All rights reserved.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
************************************************************************/


#include "SceneSerializer.h"
#include "../ecs/Scene.h"


namespace marengine {


	void FSceneSerializer::saveSceneToFile(const char* path, Scene* scene) {
		std::ofstream ss(path, std::ios::out | std::ios::trunc);
		if (!ss.is_open()) {
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
			const auto& tag{ entities[i].getComponent<CTag>().tag };

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

		const auto& cTag{ entity.getComponent<CTag>() };
		saveString("CTag", "tag", cTag.tag);

		const auto& cTransform{ entity.getComponent<CTransform>() };
		saveVec3("CTransform", "position", cTransform.position);
		saveVec3("CTransform", "rotation", cTransform.rotation);
		saveVec3("CTransform", "scale", cTransform.scale);

		if (entity.hasComponent<CRenderable>()) {
			const auto& cRenderable{ entity.getComponent<CRenderable>() };
			// TODO: fix renderable save method
			saveString("CRenderable", "name", "Cube");
		}

		if (entity.hasComponent<CPointLight>()) {
			const auto& cPointLight{ entity.getComponent<CPointLight>() };
            saveVec4("CPointLight", "position", cPointLight.pointLight.position);
            saveVec4("CPointLight", "ambient", cPointLight.pointLight.ambient);
            saveVec4("CPointLight", "diffuse", cPointLight.pointLight.diffuse);
            saveVec4("CPointLight", "specular", cPointLight.pointLight.specular);
            saveFloat("CPointLight", "constant", cPointLight.pointLight.constant);
            saveFloat("CPointLight", "linear", cPointLight.pointLight.linear);
            saveFloat("CPointLight", "quadratic", cPointLight.pointLight.quadratic);
            saveFloat("CPointLight", "shininess", cPointLight.pointLight.shininess);
        }

		if (entity.hasComponent<CCamera>()) {
			const auto& cCamera{ entity.getComponent<CCamera>() };
			saveString("CCamera", "id", cCamera.id);
			saveFloat("CCamera", "Perspective", cCamera.Perspective ? 1.f : 0.f);

			// Perspective camera save
			saveFloat("CCamera", "p_fov", cCamera.p_fov);
			saveFloat("CCamera", "p_aspectRatio", cCamera.p_aspectRatio);
			saveFloat("CCamera", "p_near", cCamera.p_near);
			saveFloat("CCamera", "p_far", cCamera.p_far);

			// Orthographic camera save
			saveFloat("CCamera", "o_left", cCamera.o_left);
			saveFloat("CCamera", "o_right", cCamera.o_right);
			saveFloat("CCamera", "o_top", cCamera.o_top);
			saveFloat("CCamera", "o_bottom", cCamera.o_bottom);
			saveFloat("CCamera", "o_near", cCamera.o_near);
			saveFloat("CCamera", "o_far", cCamera.o_far);
		}

		if (entity.hasComponent<CPythonScript>()) {
			const auto& cPythonScript{ entity.getComponent<CPythonScript>() };
			saveString("CPythonScript", "path", cPythonScript.scriptsPath);
		}
	}


}
