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


#include "../public/FileManager.h"
#include "../../../Logging/Logger.h"
#include "../../../ProjectManager.h"
#include "MARJsonDefinitions.inl"
#include "../../ecs/Scene.h"


namespace marengine {

    static void loadEntity(const Entity& entity, uint32_t index, nlohmann::json& json, const std::string& sceneName);

	void FFileDeserializer::loadSceneFromFile(Scene* pScene, const std::string& path) {
	    using namespace scenejson;

		if (!FFileManager::isContainingExtension(path, "json")) {
		    MARLOG_ERR(ELoggerType::FILESYSTEM, "Path {} does not point to marscene file!", path);
			*pScene = Scene::createEmptyScene("EmptySceneNotLoaded");
		    return;
		}
		
		std::ifstream file(path);
		if (!file.is_open()) {
            MARLOG_ERR(ELoggerType::FILESYSTEM, "Path {} cannot be opened!", path);
            *pScene = Scene::createEmptyScene("EmptySceneNotLoaded");
            return;
		}

		nlohmann::json json{ nlohmann::json::parse(file) };
		file.close();

		const std::string sceneName{ json[jScene][jSceneName] };
		const float backX{ json[jScene][sceneName][jSceneBackground][jX].get<float>() };
		const float backY{ json[jScene][sceneName][jSceneBackground][jY].get<float>() };
		const float backZ{ json[jScene][sceneName][jSceneBackground][jZ].get<float>() };

        pScene->setName(sceneName);
        pScene->setBackground({ backX, backY, backZ });

		uint32_t i = 0;
		for (nlohmann::json& jsonEntity : json[jScene][sceneName][jEntity]) {
			const Entity& entity{ pScene->createEntity() };
			loadEntity(entity, i, json, sceneName);
			i++;
		}

		MARLOG_INFO(ELoggerType::FILESYSTEM, "Loaded scene {}\n-Scene {}\n-Entities {}",
              path, pScene->getName(), pScene->getEntities().size());
	}

	void loadEntity(const Entity& entity, uint32_t index, nlohmann::json& json, const std::string& sceneName) {
        using namespace scenejson;

	    auto jsonContains = [&sceneName, index, &json](const char* componentName)->bool {
			return json[jScene][sceneName][jEntity][index].contains(componentName);
		};
		auto setString = [&sceneName, index, &json](std::string& str, const char* componentName, const char* value) {
			str = json[jScene][sceneName][jEntity][index][componentName][value];
		};
        auto loadInt = [&sceneName, index, &json](const char* componentName, const char* value)->int32 {
            return json[jScene][sceneName][jEntity][index][componentName][value].get<int32>();
        };
		auto loadFloat = [&sceneName, index, &json](const char* componentName, const char* value)->float {
			return json[jScene][sceneName][jEntity][index][componentName][value].get<float>();
		};
		auto loadVec3 = [&sceneName, index, &json](const char* componentName, const char* value)->maths::vec3 {
			return {
				json[jScene][sceneName][jEntity][index][componentName][value][jX].get<float>(),
				json[jScene][sceneName][jEntity][index][componentName][value][jY].get<float>(),
				json[jScene][sceneName][jEntity][index][componentName][value][jZ].get<float>()
			};
		};
		auto loadVec4 = [&sceneName, index, &json](const char* componentName, const char* value)->maths::vec4 {
			return {
				json[jScene][sceneName][jEntity][index][componentName][value][jX].get<float>(),
				json[jScene][sceneName][jEntity][index][componentName][value][jY].get<float>(),
				json[jScene][sceneName][jEntity][index][componentName][value][jZ].get<float>(),
				json[jScene][sceneName][jEntity][index][componentName][value][jW].get<float>()
			};
		};

		auto& cTag{ entity.getComponent<CTag>() };
		setString(cTag.tag, jCTag, jCTagTag);

		auto& cTransform{ entity.getComponent<CTransform>() };
        cTransform.position = loadVec3(jCTransform, jCTransformPosition);
        cTransform.rotation = loadVec3(jCTransform, jCTransformRotation);
        cTransform.scale = loadVec3(jCTransform, jCTransformScale);

		if (jsonContains(jCRenderable)) {
			auto& cRenderable{ entity.addComponent<CRenderable>() };
			setString(cRenderable.mesh.path, jCRenderable, jCRenderablePath);
            cRenderable.mesh.type = (EMeshType)loadInt(jCRenderable, jCRenderableMeshType);
            cRenderable.color = loadVec4(jCRenderable, jCRenderableColor);
            setString(cRenderable.material.path, jCRenderable, jCRenderableMaterial);
		}

		if (jsonContains(jCPointLight)) {
			auto& cPointLight{ entity.addComponent<CPointLight>() };
			cPointLight.pointLight.ambient = loadVec4(jCPointLight, jCPointLightPosition);
			cPointLight.pointLight.ambient = loadVec4(jCPointLight, jCPointLightAmbient);
			cPointLight.pointLight.diffuse = loadVec4(jCPointLight, jCPointLightDiffuse);
			cPointLight.pointLight.specular = loadVec4(jCPointLight, jCPointLightSpecular);
			cPointLight.pointLight.constant = loadFloat(jCPointLight, jCPointLightConstant);
			cPointLight.pointLight.linear = loadFloat(jCPointLight, jCPointLightLinear);
			cPointLight.pointLight.quadratic = loadFloat(jCPointLight, jCPointLightQuadratic);
			cPointLight.pointLight.shininess = loadFloat(jCPointLight, jCPointLightShininess);
		}

		if (jsonContains(jCCamera)) {
			auto& cCamera{ entity.addComponent<CCamera>() };
			setString(cCamera.id, jCCamera, jCCameraId);
            cCamera.Perspective = loadFloat(jCCamera, jCCameraPerspective) == 1.0f;

			// Perspective parameters loading
			cCamera.p_fov = loadFloat(jCCamera, jCCameraPFov);
			cCamera.p_aspectRatio = loadFloat(jCCamera, jCCameraPAspectRatio);
			cCamera.p_near = loadFloat(jCCamera, jCCameraPNear);
			cCamera.p_far = loadFloat(jCCamera, jCCameraPFar);

			// Orthographic parameters loading
			cCamera.o_left = loadFloat(jCCamera, jCCameraOLeft);
			cCamera.o_right = loadFloat(jCCamera, jCCameraORight);
            cCamera.o_top = loadFloat(jCCamera, jCCameraOTop);
			cCamera.o_bottom = loadFloat(jCCamera, jCCameraOBottom);
			cCamera.o_near = loadFloat(jCCamera, jCCameraONear);
			cCamera.o_far = loadFloat(jCCamera, jCCameraOFar);
		}

		if (jsonContains(jCPythonScript)) {
			auto& cPythonScript{ entity.addComponent<CPythonScript>() };
			setString(cPythonScript.scriptsPath, jCPythonScript, jCPythonScriptPath);
		}
	}


}
