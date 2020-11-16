#
# Mateusz Rzeczyca
# Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
#

cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
set(CMAKE_GENERATOR_PLATFORM win32)
project(MAREngine)
set(MARENGINE_NAME "MAREngine")

list(APPEND MAR_ENGINE_INCLUDE_DIRS ".")
list(APPEND MAR_ENGINE_INCLUDE_LIBS ".")

# -------------------------------------------------------------
# SandboxMAR setup
# -------------------------------------------------------------
set(SANDBOXMAR_PATH ${CMAKE_CURRENT_SOURCE_DIR}/SandboxMAR)
set(SANDBOXMAR_NAME "SandboxMAR")
file(GLOB_RECURSE SANDBOXMAR_SOURCE_FILES
	${SANDBOXMAR_PATH}/src/*.c
	${SANDBOXMAR_PATH}/src/*.cpp
)

file(GLOB_RECURSE SANDBOXMAR_HEADER_FILES 
	${SANDBOXMAR_PATH}/src/*.h
	${SANDBOXMAR_PATH}/src/*.hpp
)

set(MAIN_SANDBOXMAR_FILE ${SANDBOXMAR_PATH}/main.cpp)
set(SANDBOXMAR_FILES ${SANDBOXMAR_SOURCE_FILES} ${SANDBOXMAR_HEADER_FILES} ${MAIN_SANDBOXMAR_FILE}) 

# -------------------------------------------------------------
# MAREngine source files
# -------------------------------------------------------------
set(MARENGINE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/MAREngine)
file(GLOB_RECURSE SOURCE_FILES 
	${MARENGINE_PATH}/src/*.c
	${MARENGINE_PATH}/src/*.cpp)
	
file(GLOB_RECURSE HEADER_FILES 
	${MARENGINE_PATH}/src/*.h
	${MARENGINE_PATH}/src/*.hpp)

set(MAR_ENGINE_FILES ${SOURCE_FILES} ${HEADER_FILES})
set(MARENGINE_INCLUDE_DIR "${MARENGINE_PATH}/include")

list(APPEND MAR_ENGINE_ALL_FILES ${MAR_ENGINE_FILES})
list(APPEND MAR_ENGINE_INCLUDE_DIRS ${MARENGINE_INCLUDE_DIR})

# -------------------------------------------------------------
# Include statically other libs
# -------------------------------------------------------------

# in cmake/ dir there are Find***.cmake files, set path to them
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# -------------------------------------------------------------
# OpenGL
# -------------------------------------------------------------
message("Looking for: OpenGL")
find_package(OpenGL REQUIRED)
if(OPENGL_FOUND)
	message(STATUS "OPENGL_INCLUDE_DIR:= ${OPENGL_INCLUDE_DIR}")
	message(STATUS "OPENGL_gl_LIBRARY:= ${OPENGL_gl_LIBRARY}")
	message(STATUS "OPENGL_LIBRARY:= ${OPENGL_LIBRARY}")
	message(STATUS "OPENGL_LIBRARIES:= ${OPENGL_LIBRARIES}")
else()
    message("Error: Cannot find OpenGL!")
endif()

list(APPEND MAR_ENGINE_ALL_LIBS ${OPENGL_gl_LIBRARY})
list(APPEND MAR_ENGINE_INCLUDE_LIBS "${CMAKE_CURRENT_SOURCE_DIR}/MAREngine/3rd_party/GL/lib")

# -------------------------------------------------------------
# GLAD
# -------------------------------------------------------------
message("Looking for: GLAD")
set(GLAD_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/MAREngine/3rd_party/GLAD")
set(GLAD_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/MAREngine/3rd_party/GLAD/include")
file(GLOB_RECURSE GLAD_CPP_FILES 
	${GLAD_DIRECTORY}/*.c
	${GLAD_DIRECTORY}/*.cpp
)
file(GLOB_RECURSE GLAD_HEADER_FILES 
	${GLAD_DIRECTORY}/*.h
	${GLAD_DIRECTORY}/*.hpp
)
set(GLAD_FILES ${GLAD_HEADER_FILES} ${GLAD_CPP_FILES})

list(APPEND MAR_ENGINE_INCLUDE_DIRS ${GLAD_INCLUDE_DIR})
list(APPEND MAR_ENGINE_ALL_FILES ${GLAD_FILES})

# -------------------------------------------------------------
# GLFW
# -------------------------------------------------------------
message("Looking for: GLFW")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
find_package(GLFW3 REQUIRED)
if(GLFW3_FOUND)
	message(STATUS "GLFW_INDLUCE_DIR:= ${GLFW3_INCLUDE_DIR}")
	message(STATUS "GLFW3_LIBRARY:= ${GLFW3_LIBRARY}")
	set(MAR_GLFW_DIR "${CMAKE_CURRENT_SOURCE_DIR}/MAREngine/3rd_party/GLFW/include")
	message(STATUS "MAR_GLFW_DIR:= ${MAR_GLFW_DIR}")
else()
	message(STATUS "Error: Cannot find GLFW3!")
endif()

list(APPEND MAR_ENGINE_ALL_LIBS ${GLFW3_LIBRARY})
list(APPEND MAR_ENGINE_INCLUDE_DIRS ${MAR_GLFW_DIR})
list(APPEND MAR_ENGINE_INCLUDE_LIBS "${CMAKE_CURRENT_SOURCE_DIR}/MAREngine/3rd_party/GLFW/lib-vc2019")

# -------------------------------------------------------------
# SDL2
# -------------------------------------------------------------
message("Looing for: SDL2")
set(SDL_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/MAREngine/3rd_party/SDL2-2.0.12/include")
message(STATUS "SDL_INCLUDE_DIR:= ${SDL_INCLUDE_DIR}")
file(GLOB_RECURSE SDL_CPP_FILES 
	${SDL_INCLUDE_DIR}/*.c
	${SDL_INCLUDE_DIR}/*.cpp
)
file(GLOB_RECURSE SDL_HEADER_FILES 
	${SDL_INCLUDE_DIR}/*.h
	${SDL_INCLUDE_DIR}/*.hpp
)
set(SDL_FILES ${SDL_CPP_FILES} ${SDL_HEADER_FILES})

list(APPEND MAR_ENGINE_ALL_FILES ${SDL_FILES})
list(APPEND MAR_ENGINE_ALL_LIBS SDL2 SDL2main SDL2test)
list(APPEND MAR_ENGINE_INCLUDE_DIRS ${SDL_INCLUDE_DIR})
list(APPEND MAR_ENGINE_INCLUDE_LIBS "${CMAKE_CURRENT_SOURCE_DIR}/MAREngine/3rd_party/SDL2-2.0.12/lib/x86")


# -------------------------------------------------------------
# spdlog
# -------------------------------------------------------------
message("Looking for: SPDLOG")
add_subdirectory(MAREngine/3rd_party/spdlog)
set(SPDLOG_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/MAREngine/3rd_party/spdlog/include")
message(STATUS "SPDLOG_INCLUDE_DIR:= ${SPDLOG_INCLUDE_DIR}")

list(APPEND MAR_ENGINE_INCLUDE_DIRS ${SPDLOG_INCLUDE_DIR})

# -------------------------------------------------------------
# pybind11
# -------------------------------------------------------------
message("Looking for: pybind11")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/MAREngine/3rd_party/pybind11)
set(PYBIND11_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/MAREngine/3rd_party/pybind11/include")
message(STATUS "PYBIND11_INCLUDE_DIR:= ${PYBIND11_INCLUDE_DIR}")

list(APPEND MAR_ENGINE_ALL_LIBS pybind11::embed)
list(APPEND MAR_ENGINE_INCLUDE_DIRS ${PYBIND11_INCLUDE_DIR})

# -------------------------------------------------------------
# python 3.8.5
# -------------------------------------------------------------
message("Looking for: python 3.8.5")
set(PYTHON385_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/MAREngine/3rd_party/python3.8.5/include")
set(PYTHON385_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/MAREngine/3rd_party/python3.8.5/libs")
message(STATUS "PYTHON385_INCLUDE_DIR:= ${PYTHON385_INCLUDE_DIR}")
message(STATUS "PYTHON385_LIB_DIR:= ${PYTHON385_LIB_DIR}")

list(APPEND MAR_ENGINE_ALL_LIBS python38)
list(APPEND MAR_ENGINE_INCLUDE_DIRS ${PYTHON385_INCLUDE_DIR})
list(APPEND MAR_ENGINE_INCLUDE_LIBS ${PYTHON385_LIB_DIR})

# -------------------------------------------------------------
# MARMaths
# -------------------------------------------------------------
message("Looking for: MARMaths")
set(MAR_MATH_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/MAREngine/3rd_party/MARMaths/include")
set(MAR_MATH_INCLUDE_FILE ${MAR_MATH_INCLUDE_DIR}/MARMaths.h)
set(MAR_MATH_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/MAREngine/3rd_party/MARMaths/src")
file(GLOB_RECURSE MAR_MATH_LIB_SOURCE_FILES 
	${MAR_MATH_DIRECTORY}/*.c
	${MAR_MATH_DIRECTORY}/*.cpp
)
file(GLOB_RECURSE MAR_MATH_LIB_HEADER_FILES 
	${MAR_MATH_DIRECTORY}/*.h
	${MAR_MATH_DIRECTORY}/*.hpp
)
set(MAR_MATH_FILES ${MAR_MATH_LIB_SOURCE_FILES} ${MAR_MATH_LIB_HEADER_FILES} ${MAR_MATH_INCLUDE_FILE})
message(STATUS "MAR_MATH_INCLUDE_DIR:= ${MAR_MATH_INCLUDE_DIR}")

list(APPEND MAR_ENGINE_ALL_FILES ${MAR_MATH_FILES})
list(APPEND MAR_ENGINE_INCLUDE_DIRS ${MAR_MATH_INCLUDE_DIR})

# -------------------------------------------------------------
# stb_image
# -------------------------------------------------------------
message("Looking for: stb_image")
set(STB_IMAGE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/MAREngine/3rd_party/stb_image/include")
set(STB_IMAGE_H "${STB_IMAGE_INCLUDE_DIR}/stb_image/stb_image.h")
set(STB_IMAGE_SRC "${STB_IMAGE_INCLUDE_DIR}/stb_image/stb_image.cpp")
set(STB_IMAGE_FILES ${STB_IMAGE_H} ${STB_IMAGE_SRC})
message(STATUS "STB_IMAGE_INCLUDE_DIR:= ${STB_IMAGE_INCLUDE_DIR}")

list(APPEND MAR_ENGINE_ALL_FILES ${STB_IMAGE_FILES})
list(APPEND MAR_ENGINE_INCLUDE_DIRS ${STB_IMAGE_INCLUDE_DIR})

# -------------------------------------------------------------
# Dear ImGui
# -------------------------------------------------------------
message("Looking for: Dear ImGui")
set(IMGUI_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/MAREngine/3rd_party/imgui")
set(IMGUI_SOURCES
	${IMGUI_INCLUDE_DIR}/examples/imgui_impl_glfw.cpp
    ${IMGUI_INCLUDE_DIR}/examples/imgui_impl_opengl3.cpp
    ${IMGUI_INCLUDE_DIR}/examples/imgui_impl_sdl.cpp
	${IMGUI_INCLUDE_DIR}/imgui.cpp
	${IMGUI_INCLUDE_DIR}/imgui_demo.cpp
	${IMGUI_INCLUDE_DIR}/imgui_draw.cpp
	${IMGUI_INCLUDE_DIR}/imgui_widgets.cpp
)
set(IMGUI_HEADERS
	${IMGUI_INCLUDE_DIR}/examples/imgui_impl_opengl3.h
    ${IMGUI_INCLUDE_DIR}/examples/imgui_impl_glfw.h
    ${IMGUI_INCLUDE_DIR}/examples/imgui_impl_sdl.h
	${IMGUI_INCLUDE_DIR}/imgui.h
	${IMGUI_INCLUDE_DIR}/imconfig.h
    ${IMGUI_INCLUDE_DIR}/imgui_internal.h
    ${IMGUI_INCLUDE_DIR}/imstb_rectpack.h
    ${IMGUI_INCLUDE_DIR}/imstb_textedit.h
    ${IMGUI_INCLUDE_DIR}/imstb_truetype.h
)
set(IMGUI_FILES ${IMGUI_HEADERS} ${IMGUI_SOURCES})
message(STATUS "IMGUI_INCLUDE_DIR:= ${IMGUI_INCLUDE_DIR}")

list(APPEND MAR_ENGINE_ALL_FILES ${IMGUI_FILES})
list(APPEND MAR_ENGINE_INCLUDE_DIRS ${IMGUI_INCLUDE_DIR})

# -------------------------------------------------------------
# ImGuizmo
# -------------------------------------------------------------
message("Looking for: ImGuizmo")
set(IMGUIZMO_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/MAREngine/3rd_party/ImGuizmo")
set(IMGUIZMO_HEADERS ${IMGUIZMO_INCLUDE_DIR}/ImGuizmo.h)
set(IMGUIZMO_SOURCES ${IMGUIZMO_INCLUDE_DIR}/ImGuizmo.cpp)
set(IMGUIZMO_FILES ${IMGUIZMO_HEADERS} ${IMGUIZMO_SOURCES})
message(STATUS "IMGUIZMO_INCLUDE_DIR:= ${IMGUIZMO_INCLUDE_DIR}")

list(APPEND MAR_ENGINE_ALL_FILES ${IMGUIZMO_FILES})
list(APPEND MAR_ENGINE_INCLUDE_DIRS ${IMGUIZMO_INCLUDE_DIR})

# -------------------------------------------------------------
# ImGuiColorTextEdit
# -------------------------------------------------------------
message("Looking for: ImGuiColorTextEdit")
set(IMGUICOLORTEXTEDIT_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/MAREngine/3rd_party/ImGuiColorTextEdit")
set(IMGUICOLORTEXTEDIT_HEADERS ${IMGUICOLORTEXTEDIT_INCLUDE_DIR}/TextEditor.h)
set(IMGUICOLORTEXTEDIT_SOURCES ${IMGUICOLORTEXTEDIT_INCLUDE_DIR}/TextEditor.cpp)
set(IMGUICOLORTEXTEDIT_FILES ${IMGUICOLORTEXTEDIT_HEADERS} ${IMGUICOLORTEXTEDIT_SOURCES})
message(STATUS "IMGUICOLORTEXTEDIT_INCLUDE_DIR:= ${IMGUICOLORTEXTEDIT_INCLUDE_DIR}")

list(APPEND MAR_ENGINE_ALL_FILES ${IMGUICOLORTEXTEDIT_FILES})
list(APPEND MAR_ENGINE_INCLUDE_DIRS ${IMGUICOLORTEXTEDIT_INCLUDE_DIR})

# -------------------------------------------------------------
# entt
# -------------------------------------------------------------
message("Looking for: entt")
set(ENTT_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/MAREngine/3rd_party/entt/include")
set(ENTT_FILES ${ENTT_INCLUDE_DIR}/entt/entt.hpp)
message(STATUS "ENTT_INCLUDE_DIR:= ${ENTT_INCLUDE_DIR}")

list(APPEND MAR_ENGINE_ALL_FILES ${ENTT_FILES})
list(APPEND MAR_ENGINE_INCLUDE_DIRS ${ENTT_INCLUDE_DIR})

# -------------------------------------------------------------
# OBJ-Loader
# -------------------------------------------------------------
message("Looking for: OBJ-Loader")
set(OBJ_LOADER_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/MAREngine/3rd_party/OBJ-Loader")
set(OBJ_LOADER_FILES ${OBJ_LOADER_INCLUDE_DIR}/Source/OBJ_Loader.h ${OBJ_LOADER_INCLUDE_DIR}/Source/OBJ_Loader.cpp)
message(STATUS "OBJ_LOADER_INCLUDE_DIR:= ${OBJ_LOADER_INCLUDE_DIR}")

list(APPEND MAR_ENGINE_ALL_FILES ${OBJ_LOADER_FILES})
list(APPEND MAR_ENGINE_INCLUDE_DIRS ${OBJ_LOADER_INCLUDE_DIR})

# -------------------------------------------------------------
# ImGuiFileDialog
# -------------------------------------------------------------
message("Looking for: ImGuiFileBrowser")
set(IMGUI_FILE_BROWSER_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/MAREngine/3rd_party/ImGui-Addons")
set(IMGUI_FILE_BROWSER_FILES
	"${IMGUI_FILE_BROWSER_INCLUDE_DIR}/FileBrowser/ImGuiFileBrowser.h"
	"${IMGUI_FILE_BROWSER_INCLUDE_DIR}/FileBrowser/ImGuiFileBrowser.cpp"
	"${IMGUI_FILE_BROWSER_INCLUDE_DIR}/FileBrowser/Dirent/dirent.h"
)
message(STATUS "IMGUI_FILE_BROWSER_INCLUDE_DIR:= ${IMGUI_FILE_BROWSER_INCLUDE_DIR}")

list(APPEND MAR_ENGINE_ALL_FILES ${IMGUI_FILE_BROWSER_FILES})
list(APPEND MAR_ENGINE_INCLUDE_DIRS ${IMGUI_FILE_BROWSER_INCLUDE_DIR})

# -------------------------------------------------------------
# Set includes for MAREngine (mar_includes.h file)
# -------------------------------------------------------------
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/MAREngine/src" ${MAR_ENGINE_INCLUDE_DIRS})

# -------------------------------------------------------------
# link .lib files for MAREngine
# -------------------------------------------------------------
link_directories(${MAR_ENGINE_INCLUDE_LIBS})

# -------------------------------------------------------------
# Create MAREngine library
# -------------------------------------------------------------
message("Creating ${MARENGINE_NAME} library...")
add_library(${MARENGINE_NAME} ${MAR_ENGINE_ALL_FILES})

message("Linking libraries to ${MARENGINE_NAME}")
target_compile_features(${MARENGINE_NAME} PRIVATE cxx_std_17)
target_link_libraries ( ${MARENGINE_NAME} PRIVATE ${MAR_ENGINE_ALL_LIBS})

# -------------------------------------------------------------
# Create SandboxMAR executable
# -------------------------------------------------------------
message("Creating ${SANDBOXMAR_NAME} executable...")
add_executable(${SANDBOXMAR_NAME} ${SANDBOXMAR_FILES} ${MARENGINE_INCLUDE_DIR})
message("Linking libraries to ${SANDBOXMAR_NAME}")
target_compile_features(${SANDBOXMAR_NAME} PRIVATE cxx_std_17)
target_link_libraries ( ${SANDBOXMAR_NAME} PRIVATE ${MARENGINE_NAME})

if(MSVC)
# -------------------------------------------------------------
# Ignore dynamic linking
# -------------------------------------------------------------
	add_compile_options(
        $<$<CONFIG:>:/MT> #---------|
        $<$<CONFIG:Debug>:/MTd> #---|-- Statically link the runtime libraries
        $<$<CONFIG:Release>:/MT> #--|
    )

	set(VS_MULTITHREADED_DEBUG_DLL_IGNORE_LIBRARY_FLAGS
	   "/NODEFAULTLIB:libc.lib
	    /NODEFAULTLIB:libcmt.lib
	    /NODEFAULTLIB:msvcrt.lib
	    /NODEFAULTLIB:libcd.lib
	    /NODEFAULTLIB:libcmtd.lib")
	set(VS_MULTITHREADED_RELEASE_DLL_IGNORE_LIBRARY_FLAGS
	   "/NODEFAULTLIB:libc.lib
	    /NODEFAULTLIB:libcmt.lib
	    /NODEFAULTLIB:libcd.lib
	    /NODEFAULTLIB:libcmtd.lib
	    /NODEFAULTLIB:msvcrtd.lib")

    set_target_properties(${PROJECT_NAME} PROPERTIES
       LINK_FLAGS_DEBUG ${VS_MULTITHREADED_DEBUG_DLL_IGNORE_LIBRARY_FLAGS}
	)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        LINK_FLAGS_RELWITHDEBINFO ${VS_MULTITHREADED_RELEASE_DLL_IGNORE_LIBRARY_FLAGS}
	)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        LINK_FLAGS_RELEASE ${VS_MULTITHREADED_RELEASE_DLL_IGNORE_LIBRARY_FLAGS}
	)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        LINK_FLAGS_MINSIZEREL ${VS_MULTITHREADED_RELEASE_DLL_IGNORE_LIBRARY_FLAGS}
	)
		
# -------------------------------------------------------------
# Organize files in Visual Studio
# -------------------------------------------------------------
	# Macro to preserve source files hierarchy in the IDE
	macro(GroupSources curdir)
		file(GLOB children RELATIVE ${PROJECT_SOURCE_DIR}/${curdir} ${PROJECT_SOURCE_DIR}/${curdir}/*)

		foreach(child ${children})
			if(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/${curdir}/${child})
				GroupSources(${curdir}/${child})
			else()
				string(REPLACE "/" "\\" groupname ${curdir})
				string(REPLACE "src" "Sources" groupname ${groupname})
				source_group(${groupname} FILES ${PROJECT_SOURCE_DIR}/${curdir}/${child})
			endif()
		endforeach()
	endmacro()

	# Run macro
	GroupSources(src)
	
endif()