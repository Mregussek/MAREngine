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
#include "../../Logging/Logger.h"
#include "MARSceneJsonDefinitions.h"
#include "../ecs/Scene.h"


namespace marengine {


	void FSceneSerializer::saveSceneToFile(const char* path, Scene* scene) {
	    using namespace marscenejson;

		std::ofstream ss(path, std::ios::out | std::ios::trunc);
		if (!ss.is_open()) {
            MARLOG_ERR(ELoggerType::FILESYSTEM, "Cannot save scene to -> {}", path);
            return;
        }

		nlohmann::json json;
		json[jDocumentation][jEngine] = "MAREngine";
		json[jDocumentation][jEngineVersion] = "v0.0.1";
		json[jDocumentation][jApp] = "DefaultApplication";
		json[jDocumentation][jAppVersion] = "v1.0.0";

		const std::string& sceneName{ scene->getName() };
		const maths::vec3& background{ scene->getBackground() };

		json[jScene][jSceneName] = sceneName;
		json[jScene][sceneName][jSceneBackground][jX] = background.x;
		json[jScene][sceneName][jSceneBackground][jY] = background.y;
		json[jScene][sceneName][jSceneBackground][jZ] = background.z;

		const std::vector<Entity>& entities{ scene->getEntities() };
		const uint32_t entitiesSize{ entities.size() };

		for (uint32_t i = 0; i < entitiesSize; i++) {
			saveEntity(entities[i], i, json, sceneName);
		}
		
		ss << json.dump(4);
		ss.close();

		MARLOG_INFO(ELoggerType::FILESYSTEM, "Saved Scene {} to -> {}", sceneName, path);
	}

	void FSceneSerializer::saveEntity(const Entity& entity, uint32_t index, nlohmann::json& json,
                                      const std::string& sceneName) {
        using namespace marscenejson;
		auto saveString = [&json, &sceneName, index](const char* componentName, const char* value,
		                                             const std::string& str) {
			json[jScene][sceneName][jEntity][index][componentName][value] = str;
		};
        auto saveInt = [&json, &sceneName, index](const char* componentName, const char* value,
                                                  int32 i) {
            json[jScene][sceneName][jEntity][index][componentName][value] = i;
        };
		auto saveFloat = [&json, &sceneName, index](const char* componentName, const char* value,
		                                            float f) {
			json[jScene][sceneName][jEntity][index][componentName][value] = f;
		};
		auto saveVec3 = [&json, &sceneName, index](const char* componentName, const char* value,
		                                           const maths::vec3& v) {
			json[jScene][sceneName][jEntity][index][componentName][value][jX] = v.x;
			json[jScene][sceneName][jEntity][index][componentName][value][jY] = v.y;
			json[jScene][sceneName][jEntity][index][componentName][value][jZ] = v.z;
		};
		auto saveVec4 = [&json, &sceneName, index](const char* componentName, const char* value,
		                                           const maths::vec4& v) {
			json[jScene][sceneName][jEntity][index][componentName][value][jX] = v.x;
			json[jScene][sceneName][jEntity][index][componentName][value][jY] = v.y;
			json[jScene][sceneName][jEntity][index][componentName][value][jZ] = v.z;
			json[jScene][sceneName][jEntity][index][componentName][value][jW] = v.w;
		};

		const auto& cTag{ entity.getComponent<CTag>() };
		saveString(jCTag, jCTagTag, cTag.tag);

		const auto& cTransform{ entity.getComponent<CTransform>() };
		saveVec3(jCTransform, jCTransformPosition, cTransform.position);
		saveVec3(jCTransform, jCTransformRotation, cTransform.rotation);
		saveVec3(jCTransform, jCTransformScale, cTransform.scale);

		if (entity.hasComponent<CRenderable>()) {
			const auto& cRenderable{ entity.getComponent<CRenderable>() };
			saveString(jCRenderable, jCRenderablePath, cRenderable.mesh.path);
			saveInt(jCRenderable, jCRenderableMeshType, (int32)cRenderable.mesh.type);
            saveVec4(jCRenderable, jCRenderableColor, cRenderable.color);
		}

		if (entity.hasComponent<CPointLight>()) {
			const auto& cPointLight{ entity.getComponent<CPointLight>() };
            saveVec4(jCPointLight, jCPointLightPosition, cPointLight.pointLight.position);
            saveVec4(jCPointLight, jCPointLightAmbient, cPointLight.pointLight.ambient);
            saveVec4(jCPointLight, jCPointLightDiffuse, cPointLight.pointLight.diffuse);
            saveVec4(jCPointLight, jCPointLightSpecular, cPointLight.pointLight.specular);
            saveFloat(jCPointLight, jCPointLightConstant, cPointLight.pointLight.constant);
            saveFloat(jCPointLight, jCPointLightLinear, cPointLight.pointLight.linear);
            saveFloat(jCPointLight, jCPointLightQuadratic, cPointLight.pointLight.quadratic);
            saveFloat(jCPointLight, jCPointLightShininess, cPointLight.pointLight.shininess);
        }

		if (entity.hasComponent<CCamera>()) {
			const auto& cCamera{ entity.getComponent<CCamera>() };
			saveString(jCCamera, jCCameraId, cCamera.id);
			saveFloat(jCCamera, jCCameraPerspective, cCamera.Perspective ? 1.f : 0.f);

			// Perspective camera save
			saveFloat(jCCamera, jCCameraPFov, cCamera.p_fov);
			saveFloat(jCCamera, jCCameraPAspectRatio, cCamera.p_aspectRatio);
			saveFloat(jCCamera, jCCameraPNear, cCamera.p_near);
			saveFloat(jCCamera, jCCameraPFar, cCamera.p_far);

			// Orthographic camera save
			saveFloat(jCCamera, jCCameraOLeft, cCamera.o_left);
			saveFloat(jCCamera, jCCameraORight, cCamera.o_right);
			saveFloat(jCCamera, jCCameraOTop, cCamera.o_top);
			saveFloat(jCCamera, jCCameraOBottom, cCamera.o_bottom);
			saveFloat(jCCamera, jCCameraONear, cCamera.o_near);
			saveFloat(jCCamera, jCCameraOFar, cCamera.o_far);
		}

		if (entity.hasComponent<CPythonScript>()) {
			const auto& cPythonScript{ entity.getComponent<CPythonScript>() };
			saveString(jCPythonScript, jCPythonScriptPath, cPythonScript.scriptsPath);
		}
	}


}
