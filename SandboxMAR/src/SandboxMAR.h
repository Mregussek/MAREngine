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

	typedef mar::engine::MAREngine MAREngine;

public:
	SandboxMAR() = default;

	void initialize() {
		m_engine.initialize();
	}

	void run() {
		window::Window window{};
		layers::LayerStack stack{};

		window.initialize(1600, 900, m_engine.getName());

		auto entityLayer = new layers::EntityLayer("Entity Layer");
		auto scene = editor::Filesystem::openFile(m_engine.getPathToLoad());

		{ // Entity Layer Setup
			entityLayer->passSceneToManager(scene);
			stack.pushLayer(entityLayer);
		}

		const auto& back = scene->getBackground();
		window.updateBackgroundColor(back);

		entityLayer->getSceneManager()->setPlayMode();

		stack.initialize();

		while (!window.isGoingToClose() && !m_engine.shouldEngineRestart())
		{
			window.clear();

			stack.update();

			window.update();
		}

		stack.close();
		window.terminate();
	}

	void shutdown() {

	}


private:

	MAREngine m_engine;

};

