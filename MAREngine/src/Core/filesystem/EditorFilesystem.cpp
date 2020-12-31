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
#include "FilesystemLogging.h"
#include "Filesystem_Saving.h"
#include "Filesystem_Loading.h"
#include "../ecs/Scene.h"
#include "../graphics/Mesh/MeshCreator.h"


namespace marengine {


	void Filesystem::saveToFile(Scene* scene, const char* filename) {
		const auto& name = scene->getName();

		FILESYSTEM_INFO("FILESYSTEM: going to save scene {} at: {}", name, filename);

		std::ofstream ss(filename, std::ios::out | std::ios::trunc);

		if (!ss.is_open()) {
			FILESYSTEM_ERROR("Cannot open file {} and save scene at {}!", name, filename);
			return;
		}

		ss << "MAREngine scene file\n";

		Filesystem_Saving::saveScene(ss, scene);
		ss.close();

		MAR_CORE_INFO("FILESYSTEM: scene has been saved!");
	}

	Scene* Filesystem::openFile(std::string filename) {
		FILESYSTEM_INFO("FILESYSTEM: going to load scene from: {}", filename);

		std::ifstream file(filename);

		if (!file.is_open()) {
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

				Filesystem_Loading::loadScene(file, scene);
			}
		}

		file.close();

		FILESYSTEM_INFO("FILESYSTEM: returning loaded scene {} from file {}", scene->getName(), filename);

		return scene;
	}

	std::string Filesystem::loadPyScript(const char* filename) {
		FILESYSTEM_INFO("FILESYSTEM: going to load python script from: {}", filename);

		std::ifstream file(filename);

		if (!file.is_open()) {
			FILESYSTEM_ERROR("Cannot open script file {}, returning empty string!", filename);
			return "";
		}

		std::string rtn{ "" };
		std::string line;

		while (std::getline(file, line)) {
			rtn += line + "\n";
		}

		file.close();

		return rtn;
	}

	void Filesystem::savePyScript(const char* filename, std::string source) {
		FILESYSTEM_INFO("FILESYSTEM: going to save PyScript at: {}", filename);

		std::ofstream ss(filename, std::ios::out | std::ios::trunc);

		if (!ss.is_open()) {
			FILESYSTEM_ERROR("FILESYSTEM: Cannot save python script - {}!", filename);
			return;
		}

		ss << std::move(source);

		ss.close();

		FILESYSTEM_INFO("FILESYSTEM: saved PyScript at: {}", filename);
	}


}
