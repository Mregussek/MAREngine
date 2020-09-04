
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