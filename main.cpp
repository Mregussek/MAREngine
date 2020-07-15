
#include "src/MAREngine.h"

int main() {

	using namespace mar::engine;

	MAREngine::getEngine()->initialize();
	MAREngine::getEngine()->run();
	MAREngine::getEngine()->exit();

	return 0;
}