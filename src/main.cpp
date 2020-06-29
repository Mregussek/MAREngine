
#include "MAREngine.h"

int main() {

	mar::engine::MAREngine::getEngine()->initialize();
	mar::engine::MAREngine::getEngine()->run();
	mar::engine::MAREngine::getEngine()->exit();

	return 0;
}