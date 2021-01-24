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

		auto& tagComponent{ entity.getComponent<TagComponent>() };
		setString(tagComponent.tag, "TagComponent", "tag");

		auto& transformComponent{ entity.getComponent<TransformComponent>() };
		transformComponent.position = loadVec3("TransformComponent", "position");
		transformComponent.rotation = loadVec3("TransformComponent", "rotation");
		transformComponent.scale = loadVec3("TransformComponent", "scale");

		if (jsonContains("RenderableComponent")) {
			auto& renderableComponent{ entity.addComponent<RenderableComponent>() };
			setString(renderableComponent.name, "RenderableComponent", "name");
			fillRenderable(renderableComponent);
		}

		if (jsonContains("ColorComponent")) {
			auto& colorComponent{ entity.addComponent<ColorComponent>() };
			colorComponent.color = loadVec4("ColorComponent", "color");
		}

		if (jsonContains("Texture2DComponent")) {
			auto& texture2DComponent{ entity.addComponent<Texture2DComponent>() };
			setString(texture2DComponent.texturePath, "Texture2DComponent", "texturePath");
		}

		if (jsonContains("TextureCubemapComponent")) {
			auto& cubemapComponent{ entity.addComponent<TextureCubemapComponent>() };
			setString(cubemapComponent.texturePath, "TextureCubemapComponent", "texturePath");
		}

		if (jsonContains("PointLightComponent")) {
			auto& pointLightComponent{ entity.addComponent<PointLightComponent>() };
			pointLightComponent.pointLight.ambient = loadVec4("PointLightComponent", "position");
			pointLightComponent.pointLight.ambient = loadVec4("PointLightComponent", "ambient");
			pointLightComponent.pointLight.diffuse = loadVec4("PointLightComponent", "diffuse");
			pointLightComponent.pointLight.specular = loadVec4("PointLightComponent", "specular");
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


}