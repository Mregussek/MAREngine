
#include "src/SandboxMAR.h"

int main() {
	SandboxMAR sandbox;

	sandbox.initialize();
	sandbox.run();
	sandbox.shutdown();
}