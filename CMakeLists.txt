#
# Mateusz Rzeczyca
# Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
#

cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(MAREngine)

link_directories(
    ${CMAKE_SOURCE_DIR}/3rd_party/GL/lib
    ${CMAKE_SOURCE_DIR}/3rd_party/GLFW/lib-vc2019
)

file(GLOB_RECURSE SOURCE_FILES 
	${CMAKE_SOURCE_DIR}/src/*.c
	${CMAKE_SOURCE_DIR}/src/*.cpp
    ${CMAKE_SOURCE_DIR}/other/imgui/examples/imgui_impl_glfw.cpp
    ${CMAKE_SOURCE_DIR}/other/imgui/examples/imgui_impl_opengl3.cpp
	${CMAKE_SOURCE_DIR}/other/imgui/imgui.cpp
	${CMAKE_SOURCE_DIR}/other/imgui/imgui_demo.cpp
	${CMAKE_SOURCE_DIR}/other/imgui/imgui_draw.cpp
	${CMAKE_SOURCE_DIR}/other/imgui/imgui_widgets.cpp
)
	
file(GLOB_RECURSE HEADER_FILES 
	${CMAKE_SOURCE_DIR}/src/*.h
	${CMAKE_SOURCE_DIR}/src/*.hpp
    ${CMAKE_SOURCE_DIR}/other/imgui/examples/imgui_impl_opengl3.h
    ${CMAKE_SOURCE_DIR}/other/imgui/examples/imgui_impl_glfw.h
	${CMAKE_SOURCE_DIR}/other/imgui/imgui.h
	${CMAKE_SOURCE_DIR}/other/imgui/imconfig.h
    ${CMAKE_SOURCE_DIR}/other/imgui/imgui_internal.h
    ${CMAKE_SOURCE_DIR}/other/imgui/imstb_rectpack.h
    ${CMAKE_SOURCE_DIR}/other/imgui/imstb_textedit.h
    ${CMAKE_SOURCE_DIR}/other/imgui/imstb_truetype.h
)

include_directories(
	"${CMAKE_SOURCE_DIR}/src"
    "${CMAKE_SOURCE_DIR}/3rd_party/glad/include"
	"${CMAKE_SOURCE_DIR}/3rd_party/GL/include"
	"${CMAKE_SOURCE_DIR}/3rd_party/glad/include"
	"${CMAKE_SOURCE_DIR}/3rd_party/GLFW/include"
	"${CMAKE_SOURCE_DIR}/3rd_party/glm"
	"${CMAKE_SOURCE_DIR}/other/stb_image/include"
	"${CMAKE_SOURCE_DIR}/other/imgui"
	"${CMAKE_SOURCE_DIR}/other/serialport/include"
	"${CMAKE_SOURCE_DIR}/other/spdlog/include"
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# OpenGL
message("Looking for: OpenGL")
find_package(OpenGL REQUIRED)

# GLM
message("Looking for: GLM")
find_package(GLM REQUIRED)

# GLFW
message("Looking for: GLFW")
find_package(GLFW3 REQUIRED)
message(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")

# spdlog
message("Looking for: SPDLOG")
add_subdirectory(other/spdlog)

# STB_IMAGE
message("Looking for: STB_IMAGE")
add_library(STB_IMAGE "${CMAKE_SOURCE_DIR}/other/stb_image/include/stb_image/stb_image.cpp")

# GLAD
message("Looking for: GLAD")
add_library(GLAD "${CMAKE_SOURCE_DIR}/other/glad/glad.c")

set(LIBS glfw3 opengl32 STB_IMAGE GLAD)

message("Linking libraries to MAREngine")
target_link_libraries(${PROJECT_NAME} ${LIBS})

if(MSVC_IDE)
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

if(WIN32)
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${PROJECT_SOURCE_DIR}/dlls"
		$<TARGET_FILE_DIR:${PROJECT_NAME}>)
endif()