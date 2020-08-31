
#include "MAREngine/MAREngine.h"

using namespace mar;

class SandboxMAR {
	const int m_width = 1600;
	const int m_height = 900;
	const char* m_name = "Sandbox MAREngine";
	std::string m_pathToScene = "resources/mar_files/empty.marscene";
	engine::MAREngine* engine{ engine::MAREngine::getEngine() };

public:
	SandboxMAR() = default;

	void initialize() {
		std::string name = m_pathToScene + " --- " + m_name;
		engine->initWindow(m_height, m_width, name.c_str());
		engine->setLoadPath(m_pathToScene);

		scripting::PythonScript::appendCurrentPath();
	}

	void run() {
		auto stack = engine->createLayerStack();
		auto entitylayer = engine->createEntityLayer();
		auto loaded_scene = engine->loadSceneFromFile();

		entitylayer->initialize(loaded_scene);
		stack.pushLayer(entitylayer);

		entitylayer->getSceneManager()->setPlayMode();

		while (engine->shouldWindowClose() && !engine->shouldEngineRestart())
		{
			engine->clearWindowScreen();

			stack.update();

			engine->resetStatistics();

			engine->swapWindowBuffers();
		}

		stack.close();
	}

	void shutdown() {
		engine->closeWindow();
	}


};

