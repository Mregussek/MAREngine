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


namespace mar::layers { 
	class EntityLayer; class LayerGUI; /* forward declarations */ 
}
namespace mar::engine {


	class MAREngine {
		std::string m_projectName{ "DefaultProject" };
		std::string m_projectPath{ "DefaultProject/" };
		std::string m_assetsPath{ "DefaultProject/Assets/" };
		std::string m_scenesPath{ "DefaultProject/Scenes/" };
		std::string m_pathLoad{ "DefaultProject/Scenes/DefaultProject.marscene" };
		std::string m_editorName{ "EditorMAR" };
		bool m_shouldRestart{ false };

		static MAREngine* main_instance;

	public:
		MAREngine() = default;

		// --- GET METHODS --- //

		static MAREngine* getEngine() { return main_instance; }
		const char* getName() { return m_editorName.c_str(); }
		std::string getPathToLoad() { return m_pathLoad; }
		std::string getProjectName() { return m_projectName; }
		std::string getProjectPath() { return m_projectPath; }
		std::string getAssetsPath() { return m_assetsPath; }
		std::string getScenesPath() { return m_scenesPath; }

		// -----------------------------------------
		// END-USER METHODS
		// -----------------------------------------

		// --- ENGINE MANAGEMENT --- //

		void setRestart() { m_shouldRestart = true; }
		void setNoRestart() { m_shouldRestart = false; }
		const bool shouldEngineRestart() { return m_shouldRestart; }
		void setLoadPath(std::string path) { m_pathLoad = path; }
		void setProjectName(std::string name) { m_projectName = name; }
		void setProjectPath(std::string path) { 
			m_projectPath = path; 
			m_assetsPath = path + "Assets/";
			m_scenesPath = path + "Scenes/";
		}

		void initialize();
		
		void connectEntityLayerToGui(layers::LayerGUI* guilayer, layers::EntityLayer* entitylayer);

		void resetStatistics();

	};


}

#endif // !MAR_ENGINE_MAIN_H