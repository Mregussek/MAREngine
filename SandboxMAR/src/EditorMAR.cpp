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


#include "EditorMAR.h"


namespace marengine {


	void EditorMAR::initialize() {
		const std::string projectName = "DefaultProject";
		const std::string sceneName = "default.marscene.json";

		m_engine.initialize(projectName, sceneName);
	}

	void EditorMAR::runProjectOnEngine() {
		WindowInstance<GLFWwindow> displayWindow{};
		LayerStack stack{};

		displayWindow.initialize(1600, 900, m_engine.getWindowName().c_str());

		auto* scene = FSceneDeserializer::loadSceneFromFile(m_engine.getStartupSceneFilename());

		RenderLayer renderLayer("Render Layer");
		renderLayer.initialize();
		stack.pushLayer(&renderLayer);

		SceneLayer sceneLayer("Scene Layer");
		sceneLayer.passSceneToManager(scene);
		sceneLayer.initialize();
		stack.pushLayer(&sceneLayer);

		FImGuiEditorLayer editorLayer;
        editorLayer.create(sceneLayer.getSceneManager(), renderLayer.getRenderStats());
		stack.pushOverlay(&editorLayer);

		while (!displayWindow.isGoingToClose() && !m_engine.shouldEngineRestart()) {
			SetupOpenGL::clearScreen(scene->getBackground());

			editorLayer.renderToViewport();

			stack.update();

			displayWindow.swapBuffers();
		}

		stack.close();
		displayWindow.terminate();
	}

	void EditorMAR::shutdown() {
		while (m_engine.shouldEngineRestart()) {
			m_engine.setNoRestart();
			runProjectOnEngine();
		}
	}


}
