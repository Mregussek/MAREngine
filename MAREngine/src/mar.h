// --- Import GLEW lib statically --- //
#define GLEW_STATIC

// Decide wheter you want import GLEW or GLAD (comment to use GLAD)
//#define IMPORT_GLEW

// --- Include OpenGL Libs --- //
#ifdef IMPORT_GLEW
#include <GL/glew.h> 
#else
#include <glad/glad.h>
#endif

#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <vec2.hpp>
#include <vec3.hpp>
#include <vec4.hpp>
#include <mat4x4.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

// --- Include other third-party libraries --- //
#pragma warning( push )
#pragma warning( disable : 26495) 
#pragma warning( disable : 6385) 
#pragma warning( disable : 6011) 
#pragma warning( disable : 6262) 
#pragma warning( disable : 6308) 
#pragma warning( disable : 4005) 

#include "../stb_image/stb_image.h"
#include "SerialPort/SerialPort.h"

#pragma warning (pop)

// --- Include C++ STL libraries --- //
#include <iostream>
#include <fstream> 
#include <vector>
#include <string>
#include <unordered_map>
#include <thread>
#include <mutex>