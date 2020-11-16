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


#include "SandboxMAR.h"


namespace mar {


	void SandboxMAR::initialize() {
		m_engine.initialize();
	}

	void SandboxMAR::run() {
		window::Window<GLFWwindow> window{};
		layers::LayerStack stack{};

		auto entityLayer = new layers::EntityLayer("Entity Layer");

		window.initialize(1600, 900, m_engine.getName());

		auto scene = editor::Filesystem::openFile(m_engine.getPathToLoad());

		{ // Entity Layer Setup
			entityLayer->passSceneToManager(scene);
			stack.pushLayer(entityLayer);
		}

		stack.initialize();

		entityLayer->getSceneManager()->setPlayMode();

		while (!window.isGoingToClose() && !m_engine.shouldEngineRestart()) {
			platforms::SetupOpenGL::clearScreen(scene->getBackground());

			stack.update();

			window.swapBuffers();
		}

		stack.close();
		window.terminate();
	}

	void SandboxMAR::shutdown() {

	}


}
