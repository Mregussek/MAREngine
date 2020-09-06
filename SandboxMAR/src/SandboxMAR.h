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


#include "MAREngine/MAREngine.h"


using namespace mar;

class SandboxMAR {
	mar::engine::MAREngine engine;

public:
	SandboxMAR() = default;

	void initialize() {
		engine = engine::MAREngine();

		engine.initialize();
	}

	void run() {
		window::Window window = window::Window();
		window.initialize(1600, 900, engine.getName());

		auto stack = layers::LayerStack();
		auto entitylayer = new layers::EntityLayer("Entity Layer");
		auto loaded_scene = editor::Filesystem::openFile(engine.getPathToLoad());

		{ // Entity Layer Setup
			entitylayer->initialize(loaded_scene);
			stack.pushLayer(entitylayer);
		}

		auto& back = entitylayer->getSceneManager()->getScene()->getBackground();
		window.updateBackgroundColor(back);

		entitylayer->getSceneManager()->setPlayMode();

		while (!window.isGoingToClose() && !engine.shouldEngineRestart())
		{
			window.clear();

			stack.update();

			engine.resetStatistics();
			window.update();
		}

		stack.close();
		window.terminate();
	}

	void shutdown() {

	}


};

