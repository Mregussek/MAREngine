
#ifndef DEBUG_H
#define DEBUG_H

#include "mar.h"

void glCheckError_(const char* file, int line) {
    GLenum errorCode;
    errorCode = glGetError();

        std::string error;
        switch (errorCode)
        {
        case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
        case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
        case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
        case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
        case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
        case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
        case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        default:  error = "NO ERROR";
        }

        std::cout << error << " | " << file << " | " << line << std::endl;
}

#define glCheckError() glCheckError_(__FILE__, __LINE__) 

#endif // DEBUG_H