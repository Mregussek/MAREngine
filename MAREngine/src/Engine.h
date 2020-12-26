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


#ifndef MAR_ENGINE_MAIN_H
#define MAR_ENGINE_MAIN_H


#include "mar.h"


namespace marengine {


	class MAREngine {
	public:

		MAREngine() = default;

		// --- STATE CHECKS --- // 

		const bool shouldEngineRestart() const { return m_shouldRestart; }

		// --- GET METHODS --- //

		static MAREngine* Instance() { return s_instance; }
		const char* getName() const { return m_editorName.c_str(); }
		const std::string& getPathToLoad() const { return m_pathLoad; }
		const std::string& getProjectName() const { return m_projectName; }
		const std::string& getProjectPath() const { return m_projectPath; }
		const std::string& getAssetsPath() const { return m_assetsPath; }
		const std::string& getScenesPath() const { return m_scenesPath; }

		// --- SET METHODS --- //

		void setRestart() { m_shouldRestart = true; }
		void setNoRestart() { m_shouldRestart = false; }
		void setLoadPath(std::string path) { m_pathLoad = path; }
		void setProjectName(std::string name) { m_projectName = name; }
		void setProjectPath(std::string path) { 
			m_projectPath = path; 
			m_assetsPath = path + "Assets/";
			m_scenesPath = path + "Scenes/";
		}

		void initialize();
		
	private:

		std::string m_projectName{ "DefaultProject" };
		std::string m_projectPath{ "DefaultProject/" };
		std::string m_assetsPath{ "DefaultProject/Assets/" };
		std::string m_scenesPath{ "DefaultProject/Scenes/" };
		std::string m_pathLoad{ "DefaultProject/Scenes/DefaultProject.marscene" };
		std::string m_editorName{ "EditorMAR" };
		bool m_shouldRestart{ false };

		static MAREngine* s_instance;

	};


}

#endif // !MAR_ENGINE_MAIN_H