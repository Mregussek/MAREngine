
#include "mre.h"

#define CHECKERROR(x) GLClearError();\
	x;\
	if(!(GLLogCall(#x, __FILE__, __LINE__))) __debugbreak();

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);