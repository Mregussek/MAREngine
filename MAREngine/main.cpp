
// Decide wheter you want to run learning code or MAREngine (comment to use MAREngine)
//#define LEARNING_PURPOSES

#ifdef LEARNING_PURPOSES
#include "tests/learn.h"
#else
#include "src/Application.h"
#endif

int main() {
#ifdef LEARNING_PURPOSES
	return learn();
#else
	mar::Application app;
	return app.run();
#endif
}