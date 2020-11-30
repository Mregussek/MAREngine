

#pragma warning( push )
#pragma warning( disable : 28612 )

#define MAR_ENGINE_USE_VOLK 1

#if MAR_ENGINE_USE_VOLK

#if __has_include(<volk.h>)
	#include <volk.h> 
	#define MAR_ENGINE_VOLK_LIB_IMPORTED
#else
	#error "MAR ENGINE: Cannot import volk.h!"
#endif

#else

#if __has_include(<vulkan/vulkan.h>)
	#include <vulkan/vulkan.h> 
	#define MAR_ENGINE_VULKAN_LIB_IMPORTED
#else
	#error "MAR ENGINE: Cannot import vulkan/vulkan.h!"
#endif

#endif

#pragma warning (pop)


#ifndef NOMINAX
	#define NOMINAX
#endif

#undef max
#undef min

#include <iostream>
#include <vector>
#include <array>
#include <algorithm>

#if __has_include(<glad/glad.h>)
	#include <glad/glad.h> 
	#define MAR_ENGINE_GLAD_LIB_IMPORTED
	#undef APIENTRY
#else
	#error "MAR ENGINE: Cannot import glad/glad.h!"
#endif

#if __has_include(<GLFW/glfw3.h>)
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#define MAR_ENGINE_GLFW_LIB_IMPORTED
#else
#error "MAR ENGINE: Cannot import GLFW/glfw3.h!"
#endif

#if __has_include("OBJ_Loader.h")
#include "OBJ_Loader.h"
#define MAR_ENGINE_OBJ_LOADER_LIB_IMPORTED
#else
#error "MAR ENGINE: Cannot import OBJ_Loader lib - OBJ_Loader.h!"
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