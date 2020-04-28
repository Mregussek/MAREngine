
#ifndef DEBUG_H
#define DEBUG_H

#include "mre.h"

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line) {
	while (GLenum error = glGetError()) {
		std::cout << "[" << error << "] : " <<
			function << " : " << file << " : " << line << std::endl;
		return false;
	}

	return true;
}

#endif // DEBUG_H