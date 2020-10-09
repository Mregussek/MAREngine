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


#include "EditorFilesystem.h"
#include "../../Core/graphics/Mesh/MeshCreator.h"
#include "../EditorLogging.h"
#include "Filesystem_Saving.h"
#include "Filesystem_Loading.h"


namespace mar::editor {


	void Filesystem::saveToFile(ecs::Scene* scene, const char* filename) {
		EDITOR_INFO("FILESYSTEM: going to save scene {} at: {}", scene->getName(), filename);

		std::ofstream ss(filename, std::ios::out | std::ios::trunc);

		if (!ss.is_open()) {
			EDITOR_ERROR("Cannot open file {} and save scene at {}!", scene->getName(), filename);
			return;
		}

		std::string name = scene->getName();
		if (name == "EmptyScene")
			name = std::string(filename);

		ss << "MAREngine scene file\n";

		auto& back = scene->getBackground();

		Filesystem_Saving::saveScene(ss, scene);

		MAR_CORE_INFO("FILESYSTEM: scene has been saved!");
	}

	ecs::Scene* Filesystem::openFile(std::string filename) {
		EDITOR_INFO("FILESYSTEM: going to load scene from: {}", filename);

		std::ifstream file(filename);
		if (!file.is_open()) {
			if (filename == "BrandNewScene") {
				return ecs::Scene::createEmptyScene("BrandNewScene");
			}
			else {
				EDITOR_ERROR("Cannot open file {}, returning empty scene!", filename);
				return ecs::Scene::createEmptyScene("EmptySceneNotLoaded");
			}
		}

		std::string line;
		ecs::Scene* scene{ nullptr };

		while (std::getline(file, line)) {
			if (line.find("#SceneStart") != std::string::npos) {
				std::getline(file, line); // next line should be #Scene_Name -- 11
				std::istringstream is(line.substr(11));
				std::string name;
				is >> name;
				scene = new ecs::Scene(name);
				
				std::getline(file, line); // next line should be #Scene_Background -- 18
				float arr[3];
				is.clear();
				is = std::istringstream(line.substr(18));
				is >> arr[0] >> arr[1] >> arr[2];
				scene->setBackground({ arr[0], arr[1], arr[2] });

				Filesystem_Loading::loadScene(file, scene);
			}
		}

		file.close();

		EDITOR_INFO("FILESYSTEM: returning loaded scene {} from file {}", scene->getName(), filename);

		return scene;
	}

	std::string Filesystem::loadPyScript(const char* filename) {
		EDITOR_INFO("FILESYSTEM: going to load python script from: {}", filename);

		std::ifstream file(filename);
		if (!file.is_open()) {
			EDITOR_ERROR("Cannot open script file {}, returning empty string!", filename);
			return "";
		}

		std::string rtn{ "" };
		std::string line;

		while (std::getline(file, line)) {
			rtn += line + "\n";
		}

		return rtn;
	}

	void Filesystem::savePyScript(const char* filename, std::string source) {
		EDITOR_INFO("FILESYSTEM: going to save PyScript at: {}", filename);

		std::ofstream ss(filename, std::ios::out | std::ios::trunc);

		if (!ss.is_open()) {
			EDITOR_ERROR("FILESYSTEM: Cannot save python script - {}!", filename);
			return;
		}

		ss << std::move(source);

		ss.close();

		EDITOR_INFO("FILESYSTEM: saved PyScript at: {}", filename);
	}


}
