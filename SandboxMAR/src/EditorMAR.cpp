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
		m_engine.initialize();

		//projectSelectionWindow();
	}
	/*
	void EditorMAR::projectSelectionWindow() {
		window::Window<GLFWwindow> window{};
		window.initialize(1200, 700, "MAREngine - Project Manager - (C) 2020-present Mateusz Rzeczyca");

		editor::ProjectSelectionGUI gui;

		gui.initialize("#version 330");

		while (!window.isGoingToClose())
		{
			platforms::SetupOpenGL::clearScreen({0.f, 1.0f, 1.0f});

			gui.prepare();
			gui.update();
			gui.render();

			window.swapBuffers();
		}

		gui.shutdown();
		window.terminate();
	}
	*/
	void EditorMAR::runProjectOnEngine() {
		WindowInstance<GLFWwindow> displayWindow{};
		LayerStack stack{};

		//auto* scene = FSceneDeserializer::xmlLoadScene("DefaultProject/Scenes/work.marscene.xml");
		auto* scene = FSceneDeserializer::loadSceneFromFile(m_engine.getPathToLoad());
		//auto* scene = FSceneDeserializer::oldWayLoadingFile("DefaultProject/Scenes/DefaultProject.marscene");
		//auto* scene = Scene::createEmptyScene("EmptyScene");

		RenderLayer renderLayer("Render Layer");
		stack.pushLayer(&renderLayer);

		SceneLayer sceneLayer("Scene Layer");
		sceneLayer.passSceneToManager(scene);
		stack.pushLayer(&sceneLayer);

		EditorLayer editorLayer("Editor Layer");
		stack.pushOverlay(&editorLayer);

		displayWindow.initialize(1600, 900, m_engine.getName());

		stack.initialize();

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
