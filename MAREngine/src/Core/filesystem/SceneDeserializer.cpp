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


#include "SceneDeserializer.h"
#include "../graphics/MeshManager.h"
#include "../../Logging/Logger.h"
#include "../../ProjectManager.h"
#include "../ecs/Scene.h"
#include "../ecs/Entity/Entity.h"
#include "../ecs/Entity/Components.h"


namespace marengine {

    FMeshManager* FSceneDeserializer::s_pMeshManager{ nullptr };


    static bool doesContainExtension(const std::string& path, std::string&& extension) {
        const std::string currentExtension{ path.substr(path.find_last_of('.') + 1) };
        if(currentExtension == extension) {
            MARLOG_TRACE(ELoggerType::FILESYSTEM, "Path {} contains {} extension", path, extension);
            return true;
        }

        MARLOG_TRACE(ELoggerType::FILESYSTEM, "Path {} does not contain {} extension", path, extension);
        return false;
    }

	Scene* FSceneDeserializer::loadSceneFromFile(const std::string& path) {
		if (!doesContainExtension(path, "json")) {
		    MARLOG_ERR(ELoggerType::FILESYSTEM, "Path {} does not point to marscene file!", path);
			return Scene::createEmptyScene("EmptySceneNotLoaded");
		}
		
		std::ifstream file(path);
		if (!file.is_open()) {
            MARLOG_ERR(ELoggerType::FILESYSTEM, "Path {} cannot be opened!", path);
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

		MARLOG_INFO(ELoggerType::FILESYSTEM, "Loaded scene {}\n-Scene {}\n-Entities {}", path, scene->getName(), scene->getEntities().size());
		return scene;
	}

	static bool fillRenderable(FMeshManager* pManager, const std::string& renderableName,
                               CRenderable& cRenderable) {
		if (renderableName.find("Cube") != std::string::npos) {
            cRenderable.mesh.index = g_MeshDefaultTypeIndex;
            cRenderable.mesh.type = EMeshType::CUBE;
			return true;
		}
		else if (renderableName.find("Surface") != std::string::npos) {
            cRenderable.mesh.index = g_MeshDefaultTypeIndex;
            cRenderable.mesh.type = EMeshType::SURFACE;
            return true;
		}
		else if (renderableName.find("Pyramid") != std::string::npos) {
            cRenderable.mesh.index = g_MeshDefaultTypeIndex;
            cRenderable.mesh.type = EMeshType::PYRAMID;
            return true;
		}
		else if(doesContainExtension(renderableName, "obj")) {
            FMeshProxy* pMesh =
                    pManager->getFactory()->emplaceExternal(FProjectManager::getAssetsPath() + renderableName);
            cRenderable.mesh.index = pMesh->getIndex();
            cRenderable.mesh.type = EMeshType::EXTERNAL;
            return true;
        }

		return false;
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
		auto loadVec3 = [&sceneName, index, &json](const char* componentName, const char* value)->maths::vec3 {
			return {
				json["Scene"][sceneName]["Entity"][index][componentName][value]["x"].get<float>(),
				json["Scene"][sceneName]["Entity"][index][componentName][value]["y"].get<float>(),
				json["Scene"][sceneName]["Entity"][index][componentName][value]["z"].get<float>()
			};
		};
		auto loadVec4 = [&sceneName, index, &json](const char* componentName, const char* value)->maths::vec4 {
			return {
				json["Scene"][sceneName]["Entity"][index][componentName][value]["x"].get<float>(),
				json["Scene"][sceneName]["Entity"][index][componentName][value]["y"].get<float>(),
				json["Scene"][sceneName]["Entity"][index][componentName][value]["z"].get<float>(),
				json["Scene"][sceneName]["Entity"][index][componentName][value]["w"].get<float>()
			};
		};
        auto setRenderable = [&sceneName, index, &json](FMeshManager* pManager,
                                                        CRenderable& renderable,
                                                        const char* componentName,
                                                        const char* value) {
            const std::string retrievedRenderableName =
                    json["Scene"][sceneName]["Entity"][index][componentName][value];
            fillRenderable(pManager, retrievedRenderableName, renderable);
        };

		auto& cTag{ entity.getComponent<CTag>() };
		setString(cTag.tag, "CTag", "tag");

		auto& cTransform{ entity.getComponent<CTransform>() };
        cTransform.position = loadVec3("CTransform", "position");
        cTransform.rotation = loadVec3("CTransform", "rotation");
        cTransform.scale = loadVec3("CTransform", "scale");

		if (jsonContains("CRenderable")) {
			auto& cRenderable{ entity.addComponent<CRenderable>() };
			setRenderable(s_pMeshManager, cRenderable, "CRenderable", "name");
		}

		if (jsonContains("CPointLight")) {
			auto& cPointLight{ entity.addComponent<CPointLight>() };
			cPointLight.pointLight.ambient = loadVec4("CPointLight", "position");
			cPointLight.pointLight.ambient = loadVec4("CPointLight", "ambient");
			cPointLight.pointLight.diffuse = loadVec4("CPointLight", "diffuse");
			cPointLight.pointLight.specular = loadVec4("CPointLight", "specular");
			cPointLight.pointLight.constant = loadFloat("CPointLight", "constant");
			cPointLight.pointLight.linear = loadFloat("CPointLight", "linear");
			cPointLight.pointLight.quadratic = loadFloat("CPointLight", "quadratic");
			cPointLight.pointLight.shininess = loadFloat("CPointLight", "shininess");
		}

		if (jsonContains("CCamera")) {
			auto& cCamera{ entity.addComponent<CCamera>() };
			setString(cCamera.id, "CCamera", "id");
            cCamera.Perspective = loadFloat("CCamera", "Perspective") == 1.0f;

			// Perspective parameters loading
			cCamera.p_fov = loadFloat("CCamera", "p_fov");
			cCamera.p_aspectRatio = loadFloat("CCamera", "p_aspectRatio");
			cCamera.p_near = loadFloat("CCamera", "p_near");
			cCamera.p_far = loadFloat("CCamera", "p_far");

			// Orthographic parameters loading
			cCamera.o_left = loadFloat("CCamera", "o_left");
			cCamera.o_right = loadFloat("CCamera", "o_right");
			cCamera.o_bottom = loadFloat("CCamera", "o_bottom");
			cCamera.o_near = loadFloat("CCamera", "o_near");
			cCamera.o_far = loadFloat("CCamera", "o_far");
		}

		if (jsonContains("CPythonScript")) {
			auto& cPythonScript{ entity.addComponent<CPythonScript>() };
			setString(cPythonScript.scriptsPath, "CPythonScript", "path");
		}
	}


}
