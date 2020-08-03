
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
		engine->initWindow(m_height, m_width, m_name);
		engine->setLoadPath(m_pathToScene);
	}

	void run() {
		auto stack = engine->createLayerStack();
		auto entitylayer = engine->createEntityLayer();
		auto loaded_scene = engine->loadSceneFromFile();

		entitylayer->initialize(loaded_scene);
		stack.pushLayer(entitylayer);

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

