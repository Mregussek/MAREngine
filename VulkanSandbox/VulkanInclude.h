


#include "vulkan/vulkan.h"


#include <iostream>
#include <array>
#include <algorithm>

#ifndef NOMINMAX
#define NOMINMAX
#endif

#if __has_include(<GL/glew.h>)
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif
#include <GL/glew.h> 
#define MAR_ENGINE_GLEW_LIB_IMPORTED
#else
#error "MAR ENGINE: Cannot import GL/glew.h!"
#endif

#if __has_include(<GLFW/glfw3.h>)
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#define MAR_ENGINE_GLFW_LIB_IMPORTED
#else
#error "MAR ENGINE: Cannot import GLFW/glfw3.h!"
#endif

#if __has_include("Source/OBJ_Loader.h")
#include "Source/OBJ_Loader.h"
#define MAR_ENGINE_OBJ_LOADER_LIB_IMPORTED
#else
#error "MAR ENGINE: Cannot import OBJ_Loader lib - Source/OBJ_Loader.h!"
#endif

#if __has_include("MARMaths.h")
#include "MARMaths.h"
#define MAR_ENGINE_MAR_MATHS_LIB_IMPORTED
#else
#error "MAR ENGINE: Cannot import MARMaths.h!"
#endif

#ifdef WIN32
#if __has_include(<crtdbg.h>)
#include <crtdbg.h>
#else	
#error "MAR ENGINE: Cannot import crtdbg.h for Windows Platform!"
#endif
#endif