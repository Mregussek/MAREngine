
#include "MAREngine.h"

int main() {
	mar::engine::MAREngine engine;

	engine.initialize();
	engine.run();
	engine.shutdown();

	return 0;
}