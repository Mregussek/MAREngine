
#include <string>

namespace mar {
	namespace engine {
		class MAREngine;
	}
}

class EditorMAR {
	const int m_width = 1600;
	const int m_height = 900;
	std::string m_name = "Editor MAREngine";
	std::string m_pathToScene = "resources/mar_files/empty.marscene";
	mar::engine::MAREngine* engine;

public:
	EditorMAR() = default;

	void initialize();

	void run();

	void shutdown();

};

