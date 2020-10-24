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

using namespace mar;

void EditorMAR::initialize() {
	engine = engine::MAREngine();

	engine.initialize();

	//projectSelectionWindow();
}

void EditorMAR::projectSelectionWindow() {
	window::Window window = window::Window();
	window.initialize(1200, 700, "MAREngine - Project Manager - (C) 2020-present Mateusz Rzeczyca");

	editor::ProjectSelectionGUI gui;

	gui.initialize("#version 330");

	while (!window.isGoingToClose())
	{
		window.clear();

		gui.prepare();
		gui.update();
		gui.render();

		window.update();
	}

	gui.shutdown();
	window.terminate();
}

void EditorMAR::runProjectOnEngine() {
	auto window = window::Window();
	window.initialize(1600, 900, engine.getName());

	auto stack = layers::LayerStack();
	auto entityLayer = new layers::EntityLayer("Entity Layer");
	auto guiLayer = new layers::LayerGUI("Default GUI Layer");
	auto gui = editor::GUI();
	auto loaded_scene = editor::Filesystem::openFile(engine.getPathToLoad());

	{ // Entity Layer Setup
		entityLayer->initialize(loaded_scene);
		stack.pushLayer(entityLayer);
	}
	
	{ // Editor Layer Setup
		guiLayer->initialize(&gui, loaded_scene->getBackground());
		engine.connectEntityLayerToGui(guiLayer, entityLayer);
		stack.pushOverlay(guiLayer);
	}

	while (!window.isGoingToClose() && !engine.shouldEngineRestart())
	{
		window.clear();

		guiLayer->renderToViewport();

		stack.update();

		window.update();
	}

	stack.close();
	window.terminate();
}

void EditorMAR::shutdown() {
start_again:
	if (engine.shouldEngineRestart()) {
		engine.setNoRestart();
		runProjectOnEngine();
		goto start_again;
	}
}