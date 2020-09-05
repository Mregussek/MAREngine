/**
 *				MAREngine - open source 3D game engine
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


#include "MAREngine/MAREngine.h"


using namespace mar;

class SandboxMAR {
	const int m_width = 1600;
	const int m_height = 900;
	const char* m_name = "Sandbox MAREngine";
	std::string m_pathToScene = "resources/mar_files/empty.marscene";
	engine::MAREngine* engine{ engine::MAREngine::getEngine() };

public:
	SandboxMAR() = default;

	void initialize() {
		std::string name = m_pathToScene + " --- " + m_name;
		engine->initWindow(m_height, m_width, name.c_str());
		engine->setLoadPath(m_pathToScene);

		engine->initializeScripting();
	}

	void run() {
		auto stack = engine->createLayerStack();
		auto entitylayer = engine->createEntityLayer();
		auto loaded_scene = engine->loadSceneFromFile();

		entitylayer->initialize(loaded_scene);
		stack.pushLayer(entitylayer);

		entitylayer->getSceneManager()->setPlayMode();

		while (engine->shouldWindowClose() && !engine->shouldEngineRestart())
		{
			engine->clearWindowScreen();

			stack.update();

			engine->resetStatistics();

			engine->swapWindowBuffers();
		}

		stack.close();
	}

	void shutdown() {
		engine->closeWindow();
	}


};

