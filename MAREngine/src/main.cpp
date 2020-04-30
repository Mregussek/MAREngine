
// Decide wheter you want to run learning code or MAREngine (comment to use MAREngine)
//#define LEARNING_PURPOSES

#ifdef LEARNING_PURPOSES
#include "../tests/learn_textures.h"
#else
#include "Application.h"
#endif

int main() {
#ifdef LEARNING_PURPOSES
	return learn_textures();
#else
	return run();
#endif
}