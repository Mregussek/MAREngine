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


#include "EditorMAR.h"
#include "MAREngine/MAREngine.h"


void EditorMAR::initialize() {
	engine = mar::engine::MAREngine::getEngine();

	std::string name = m_pathToScene + " --- " + m_name;
	engine->initWindow(m_height, m_width, name.c_str());
	engine->setLoadPath(m_pathToScene);

	engine->initializeScripting();
}

void EditorMAR::run() {
	auto stack = engine->createLayerStack();
	auto entitylayer = engine->createEntityLayer();
	auto guilayer = engine->createEditorLayer();
	auto loaded_scene = engine->loadSceneFromFile();
	auto gui = engine->createGUI();
	auto gui_cam = engine->createGUICamera();

	engine->updateBackground(&gui, loaded_scene);

	entitylayer->initialize(loaded_scene);
	stack.pushLayer(entitylayer);

	guilayer->initialize(&gui, &gui_cam);
	gui.submit(entitylayer->getSceneManager());
	stack.pushOverlay(guilayer);

	auto& framebuffer = gui.getFramebuffer();

	while (!engine->shouldWindowClose() && !engine->shouldEngineRestart())
	{
		engine->clearWindowScreen();

		framebuffer.bind();
		framebuffer.clear();

		stack.update();

		engine->resetStatistics();

		engine->swapWindowBuffers();
	}

	stack.close();
}

void EditorMAR::shutdown() {
start_again:
	if (engine->shouldEngineRestart()) {
		engine->setNoRestart();
		run();
		goto start_again;
	}

	engine->closeWindow();
}