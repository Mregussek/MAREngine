#
# Mateusz Rzeczyca
# Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
#

cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(MAREngine)
set(MARENGINE_NAME "MAREngine")

# -------------------------------------------------------------
# EditorMAR setup
# -------------------------------------------------------------
set(EDITORMAR_NAME "EditorMAR")
file(GLOB_RECURSE EDITORMAR_SOURCE_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/EditorMAR/src/*.c
	${CMAKE_CURRENT_SOURCE_DIR}/EditorMAR/src/*.cpp
)

file(GLOB_RECURSE EDITORMAR_HEADER_FILES 
	${CMAKE_CURRENT_SOURCE_DIR}/EditorMAR/src/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/EditorMAR/src/*.hpp
)

set(MAIN_EDITORMAR_FILE ${CMAKE_CURRENT_SOURCE_DIR}/EditorMAR/main.cpp)

set(EDITORMAR_FILES ${EDITORMAR_SOURCE_FILES} ${EDITORMAR_HEADER_FILES} ${MAIN_EDITORMAR_FILE}) 

# -------------------------------------------------------------
# SandboxMAR setup
# -------------------------------------------------------------
set(SANDBOXMAR_NAME "SandboxMAR")
file(GLOB_RECURSE SANDBOXMAR_SOURCE_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/SandboxMAR/src/*.c
	${CMAKE_CURRENT_SOURCE_DIR}/SandboxMAR/src/*.cpp
)

file(GLOB_RECURSE SANDBOXMAR_HEADER_FILES 
	${CMAKE_CURRENT_SOURCE_DIR}/SandboxMAR/src/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/SandboxMAR/src/*.hpp
)

set(MAIN_SANDBOXMAR_FILE ${CMAKE_CURRENT_SOURCE_DIR}/SandboxMAR/main.cpp)

set(SANDBOXMAR_FILES ${SANDBOXMAR_SOURCE_FILES} ${SANDBOXMAR_HEADER_FILES} ${MAIN_SANDBOXMAR_FILE}) 

# -------------------------------------------------------------
# MAREngine source files
# -------------------------------------------------------------
file(GLOB_RECURSE SOURCE_FILES 
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
	
file(GLOB_RECURSE HEADER_FILES 
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp)

set(MAR_ENGINE_FILES ${SOURCE_FILES} ${HEADER_FILES})

set(MARENGINE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

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

# -------------------------------------------------------------
# glew
# -------------------------------------------------------------
message("Looking for: GLEW")
add_definitions(-DGLEW_STATIC)
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/3rd_party/GL")
find_package(GLEW REQUIRED)
if (GLEW_FOUND)
	message(STATUS "GLEW_INCLUDE_DIRS:= ${GLEW_INCLUDE_DIRS}")
    message(STATUS "GLEW_LIBRARIES:= ${GLEW_LIBRARIES}")
    message(STATUS "GLEW_STATIC_LIBRARIES:= ${GLEW_STATIC_LIBRARIES}")
else()
	message(STATUS "Error: Cannot find GLEW!")
endif()

# -------------------------------------------------------------
# glfw
# -------------------------------------------------------------
message("Looking for: GLFW")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
find_package(GLFW3 REQUIRED)
if(GLFW3_FOUND)
	message(STATUS "GLFW_INDLUCE_DIR:= ${GLFW3_INCLUDE_DIR}")
	message(STATUS "GLFW3_LIBRARY:= ${GLFW3_LIBRARY}")
	set(MAR_GLFW_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/GLFW/include")
	message(STATUS "MAR_GLFW_DIR:= ${MAR_GLFW_DIR}")
else()
	message(STATUS "Error: Cannot find GLFW3!")
endif()

# -------------------------------------------------------------
# spdlog
# -------------------------------------------------------------
message("Looking for: SPDLOG")
add_subdirectory(3rd_party/spdlog)
set(SPDLOG_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/spdlog/include")
message(STATUS "SPDLOG_INCLUDE_DIR:= ${SPDLOG_INCLUDE_DIR}")

# -------------------------------------------------------------
# pybind11
# -------------------------------------------------------------
message("Looking for: pybind11 && python3.8.5")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/pybind11)
set(PYBIND11_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/pybind11/include")
set(PYTHON385_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/python3.8.5/include")
set(PYTHON385_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/python3.8.5/libs")
message(STATUS "PYBIND11_INCLUDE_DIR:= ${PYBIND11_INCLUDE_DIR}")
message(STATUS "PYTHON385_INCLUDE_DIR:= ${PYTHON385_INCLUDE_DIR}")
message(STATUS "PYTHON385_LIB_DIR:= ${PYTHON385_LIB_DIR}")

# -------------------------------------------------------------
# MARMaths
# -------------------------------------------------------------
message("Looking for: MARMaths")
set(MAR_MATH_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/MARMaths/include")
set(MAR_MATH_INCLUDE_FILE ${MAR_MATH_INCLUDE_DIR}/MARMaths.h)
set(MAR_MATH_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/MARMaths/src")
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

# -------------------------------------------------------------
# stb_image
# -------------------------------------------------------------
message("Looking for: stb_image")
set(STB_IMAGE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/stb_image/include")
set(STB_IMAGE_H "${STB_IMAGE_INCLUDE_DIR}/stb_image/stb_image.h")
set(STB_IMAGE_SRC "${STB_IMAGE_INCLUDE_DIR}/stb_image/stb_image.cpp")
set(STB_IMAGE_FILES ${STB_IMAGE_H} ${STB_IMAGE_SRC})
message(STATUS "STB_IMAGE_INCLUDE_DIR:= ${STB_IMAGE_INCLUDE_DIR}")

# -------------------------------------------------------------
# Dear ImGui
# -------------------------------------------------------------
message("Looking for: Dear ImGui")
set(IMGUI_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/imgui")
set(IMGUI_SOURCES
	${IMGUI_INCLUDE_DIR}/examples/imgui_impl_glfw.cpp
    ${IMGUI_INCLUDE_DIR}/examples/imgui_impl_opengl3.cpp
	${IMGUI_INCLUDE_DIR}/imgui.cpp
	${IMGUI_INCLUDE_DIR}/imgui_demo.cpp
	${IMGUI_INCLUDE_DIR}/imgui_draw.cpp
	${IMGUI_INCLUDE_DIR}/imgui_widgets.cpp
)
set(IMGUI_HEADERS
	${IMGUI_INCLUDE_DIR}/examples/imgui_impl_opengl3.h
    ${IMGUI_INCLUDE_DIR}/examples/imgui_impl_glfw.h
	${IMGUI_INCLUDE_DIR}/imgui.h
	${IMGUI_INCLUDE_DIR}/imconfig.h
    ${IMGUI_INCLUDE_DIR}/imgui_internal.h
    ${IMGUI_INCLUDE_DIR}/imstb_rectpack.h
    ${IMGUI_INCLUDE_DIR}/imstb_textedit.h
    ${IMGUI_INCLUDE_DIR}/imstb_truetype.h
)
set(IMGUI_FILES ${IMGUI_HEADERS} ${IMGUI_SOURCES})
message(STATUS "IMGUI_INCLUDE_DIR:= ${IMGUI_INCLUDE_DIR}")

# -------------------------------------------------------------
# ImGuizmo
# -------------------------------------------------------------
message("Looking for: ImGuizmo")
set(IMGUIZMO_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/ImGuizmo")
set(IMGUIZMO_HEADERS ${IMGUIZMO_INCLUDE_DIR}/ImGuizmo.h)
set(IMGUIZMO_SOURCES ${IMGUIZMO_INCLUDE_DIR}/ImGuizmo.cpp)
set(IMGUIZMO_FILES ${IMGUIZMO_HEADERS} ${IMGUIZMO_SOURCES})
message(STATUS "IMGUIZMO_INCLUDE_DIR:= ${IMGUIZMO_INCLUDE_DIR}")

# -------------------------------------------------------------
# entt
# -------------------------------------------------------------
message("Looking for: entt")
set(ENTT_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/entt/include")
set(ENTT_FILES ${ENTT_INCLUDE_DIR}/entt/entt.hpp)
message(STATUS "ENTT_INCLUDE_DIR:= ${ENTT_INCLUDE_DIR}")

# -------------------------------------------------------------
# SerialPort
# -------------------------------------------------------------
message("Looking for: SerialPort")
set(SERIALPORT_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/serialport/include")
message(STATUS "SERIALPORT_INCLUDE_DIR:= ${SERIALPORT_INCLUDE_DIR}")

# -------------------------------------------------------------
# Set includes for MAREngine (mar_includes.h file)
# -------------------------------------------------------------
include_directories(
	"${CMAKE_CURRENT_SOURCE_DIR}/src"
	${SERIALPORT_INCLUDE_DIR}
	${IMGUI_INCLUDE_DIR}
	${IMGUIZMO_INCLUDE_DIR}
	${MAR_MATH_INCLUDE_DIR}
	${GLEW_INCLUDE_DIRS}
	${GLFW_INDLUCE_DIR}
	${MAR_GLFW_DIR}
	${STB_IMAGE_INCLUDE_DIR}
	${SPDLOG_INCLUDE_DIR}
	${ENTT_INCLUDE_DIR}
	${MARENGINE_INCLUDE_DIR}
	${PYBIND11_INCLUDE_DIR}
	${PYTHON385_INCLUDE_DIR}
)

# -------------------------------------------------------------
# link .lib files for MAREngine
# -------------------------------------------------------------
link_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/GL/lib
    ${CMAKE_CURRENT_SOURCE_DIR}/3rd_party/GLFW/lib-vc2019
	${CMAKE_CURRENT_SOURCE_DIR}/bin/Release/Win32/Engine
	${PYTHON385_LIB_DIR}
)

# -------------------------------------------------------------
# Imports setup
# -------------------------------------------------------------
set(ALL_INCLUDE_FILES_MARENGINE 
	${MAR_ENGINE_FILES} ${MAR_MATH_FILES} ${IMGUI_FILES} ${IMGUIZMO_FILES} 
	${STB_IMAGE_FILES} ${IMGUI_FILES} ${ENTT_FILES}
)

set(MAR_OPENGL_IMPORT 
	${OPENGL_gl_LIBRARY} ${GLEW_STATIC_LIBRARIES} ${GLFW3_LIBRARY} python38 pybind11::embed
)

# -------------------------------------------------------------
# Create Python Modules
# -------------------------------------------------------------
message("Creating MAREnginePy...")
set(SCRIPT_MODULES_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src/Core/scripting)
set(ECS_MODULES_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src/Core/ecs)
pybind11_add_module(MAREnginePy ${SCRIPT_MODULES_PATH}/EmbedEngine.cpp ${ALL_INCLUDE_FILES_MARENGINE})
target_compile_features(MAREnginePy PRIVATE cxx_std_17)
target_include_directories(MAREnginePy PRIVATE ${SCRIPT_MODULES_PATH} ${ECS_MODULES_PATH})
target_link_libraries (MAREnginePy PRIVATE ${MAR_OPENGL_IMPORT})

# -------------------------------------------------------------
# Create MAREngine library and Editor / Sandbox executables
# -------------------------------------------------------------
message("Creating ${MARENGINE_NAME} library...")
add_library(${MARENGINE_NAME} ${ALL_INCLUDE_FILES_MARENGINE})

message("Linking libraries to ${MARENGINE_NAME}")
target_compile_features(${MARENGINE_NAME} PRIVATE cxx_std_17)
target_link_libraries ( ${MARENGINE_NAME} PRIVATE ${MAR_OPENGL_IMPORT})

message("Creating ${EDITORMAR_NAME} executable...")
add_executable(${EDITORMAR_NAME} ${EDITORMAR_FILES} ${ALL_INCLUDE_FILES_MARENGINE} ${MARENGINE_INCLUDE_DIR})

message("Linking libraries to ${EDITORMAR_NAME}")
target_compile_features(${EDITORMAR_NAME} PRIVATE cxx_std_17)
target_link_libraries(${EDITORMAR_NAME} PRIVATE ${MAR_OPENGL_IMPORT} ${MARENGINE_NAME})

message("Creating ${SANDBOXMAR_NAME} executable...")
add_executable(${SANDBOXMAR_NAME} ${SANDBOXMAR_FILES} ${ALL_INCLUDE_FILES_MARENGINE} ${MARENGINE_INCLUDE_DIR})
message("Linking libraries to ${SANDBOXMAR_NAME}")
target_compile_features(${SANDBOXMAR_NAME} PRIVATE cxx_std_17)
target_link_libraries ( ${SANDBOXMAR_NAME} PRIVATE ${MAR_OPENGL_IMPORT} ${MARENGINE_NAME})

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
	
# -------------------------------------------------------------
# Copy imgui.ini and /resources to build dir's
# -------------------------------------------------------------
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/resources"
		"$<TARGET_FILE_DIR:${PROJECT_NAME}>/resources")

	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/imgui.ini"
		"$<TARGET_FILE_DIR:${PROJECT_NAME}>")
		
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory "${PROJECT_SOURCE_DIR}/resources"
		"${CMAKE_SOURCE_DIR}/build/resources")

	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/imgui.ini"
		"${CMAKE_SOURCE_DIR}/build")
endif()