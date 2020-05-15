
#include "MAREngine.h"

int main() {
	mar::MAREngine engine;

	engine.initialize();
	engine.run();
	engine.shutdown();

	return 0;
}