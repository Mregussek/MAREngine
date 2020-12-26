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


namespace marengine {


	void SandboxMAR::initialize() {
		m_engine.initialize();
	}

	void SandboxMAR::run() {
		WindowInstance<GLFWwindow> displayWindow{};
		LayerStack stack{};

		auto* scene = Filesystem::openFile(m_engine.getPathToLoad());

		auto renderLayer = new RenderLayer("Render Layer");
		stack.pushLayer(renderLayer);

		auto sceneLayer = new SceneLayer("Scene Layer");
		sceneLayer->passSceneToManager(scene);
		stack.pushLayer(sceneLayer);

		displayWindow.initialize(1600, 900, m_engine.getName());

		stack.initialize();

		sceneLayer->getSceneManager()->setPlayMode();

		while (!displayWindow.isGoingToClose() && !m_engine.shouldEngineRestart()) {
			SetupOpenGL::clearScreen(scene->getBackground());

			stack.update();

			displayWindow.swapBuffers();
		}

		stack.close();
		displayWindow.terminate();
	}

	void SandboxMAR::shutdown() {

	}


}
