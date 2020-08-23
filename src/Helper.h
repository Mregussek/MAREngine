
#include "Engine.h"

using namespace mar;

class EditorMAR {
	const int m_width = 1600;
	const int m_height = 900;
	std::string m_name = "Editor MAREngine";
	std::string m_pathToScene = "resources/mar_files/empty.marscene";
	engine::MAREngine* engine{ engine::MAREngine::getEngine() };

public:
	EditorMAR() = default;

	void initialize() {
		std::string name = m_pathToScene + " --- " + m_name;
		engine->initWindow(m_height, m_width, name.c_str());
		engine->setLoadPath(m_pathToScene);

		static pybind11::scoped_interpreter guard{};
		scripting::PythonScript::appendCurrentPath();
	}

	void run() {
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

		while (engine->shouldWindowClose() && !engine->shouldEngineRestart())
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

	void shutdown() {
	start_again:
		if (engine->shouldEngineRestart()) {
			engine->setNoRestart();
			run();
			goto start_again;
		}

		engine->closeWindow();
	}


};

