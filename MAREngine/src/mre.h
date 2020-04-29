// --- Import GLEW lib statically --- //
#define GLEW_STATIC

// --- Include OpenGL Libs --- //
#include <GL/glew.h>
//#include <glad/glad.h> // we don't need glad because we have glew
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <vec2.hpp>
#include <vec3.hpp>
#include <vec4.hpp>
#include <mat4x4.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

// --- Include C++ STL libraries --- //
//#include <stdio.h>
#include <iostream>
#include <fstream> // for reading shaders
#include <vector>
#include <string>
#include <unordered_map> // for uniform location cache
#include <atomic>
#include <thread> // for serial port running parallel with engine
#include <mutex> // for safe reading coords from serial port