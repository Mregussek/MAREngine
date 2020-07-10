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

set(STB_IMAGE_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/other/stb_image/include")
set(STB_IMAGE_H "${CMAKE_SOURCE_DIR}/other/stb_image/include/stb_image/stb_image.h")
set(STB_IMAGE_SRC "${CMAKE_SOURCE_DIR}/other/stb_image/include/stb_image/stb_image.cpp")

set(IMGUI_SOURCES
	${CMAKE_SOURCE_DIR}/other/imgui/examples/imgui_impl_glfw.cpp
    ${CMAKE_SOURCE_DIR}/other/imgui/examples/imgui_impl_opengl3.cpp
	${CMAKE_SOURCE_DIR}/other/imgui/imgui.cpp
	${CMAKE_SOURCE_DIR}/other/imgui/imgui_demo.cpp
	${CMAKE_SOURCE_DIR}/other/imgui/imgui_draw.cpp
	${CMAKE_SOURCE_DIR}/other/imgui/imgui_widgets.cpp)

set(IMGUI_HEADERS
	${CMAKE_SOURCE_DIR}/other/imgui/examples/imgui_impl_opengl3.h
    ${CMAKE_SOURCE_DIR}/other/imgui/examples/imgui_impl_glfw.h
	${CMAKE_SOURCE_DIR}/other/imgui/imgui.h
	${CMAKE_SOURCE_DIR}/other/imgui/imconfig.h
    ${CMAKE_SOURCE_DIR}/other/imgui/imgui_internal.h
    ${CMAKE_SOURCE_DIR}/other/imgui/imstb_rectpack.h
    ${CMAKE_SOURCE_DIR}/other/imgui/imstb_textedit.h
    ${CMAKE_SOURCE_DIR}/other/imgui/imstb_truetype.h)

file(GLOB_RECURSE SOURCE_MATH 
	${CMAKE_SOURCE_DIR}/other/MARMaths/src/*.c
	${CMAKE_SOURCE_DIR}/other/MARMaths/src/*.cpp)
	
file(GLOB_RECURSE HEADER_MATH 
	${CMAKE_SOURCE_DIR}/other/MARMaths/src/*.h
	${CMAKE_SOURCE_DIR}/other/MARMaths/src/*.hpp)
	
set(MAIN_MATH ${CMAKE_SOURCE_DIR}/other/MARMaths/MARMaths.h)

file(GLOB_RECURSE SOURCE_FILES 
	${CMAKE_SOURCE_DIR}/src/*.c
	${CMAKE_SOURCE_DIR}/src/*.cpp)
	
file(GLOB_RECURSE HEADER_FILES 
	${CMAKE_SOURCE_DIR}/src/*.h
	${CMAKE_SOURCE_DIR}/src/*.hpp)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# OpenGL
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

# GLEW
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

# GLFW
message("Looking for: GLFW")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
find_package(GLFW3 REQUIRED)
if(GLFW3_FOUND)
	message(STATUS "GLFW_INDLUCE_DIR:= ${GLFW3_INCLUDE_DIR}")
	message(STATUS "GLFW3_LIBRARY:= ${GLFW3_LIBRARY}")
	set(MAR_GLFW_DIR "${CMAKE_SOURCE_DIR}/3rd_party/GLFW/include")
	message(STATUS "MAR_GLFW_DIR:= ${MAR_GLFW_DIR}")
else()
	message(STATUS "Error: Cannot find GLFW3!")
endif()

# spdlog
message("Looking for: SPDLOG")
add_subdirectory(other/spdlog)
set(SPDLOG_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/other/spdlog/include")
message(STATUS "SPDLOG_INCLUDE_DIR:= ${SPDLOG_INCLUDE_DIR}")

# MARMaths
message("Looking for: MARMaths")
set(MAR_MATHS_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/other/MARMaths")
message(STATUS "MAR_MATHS_INCLUDE_DIR:= ${MAR_MATHS_INCLUDE_DIR}")

include_directories(
	"${CMAKE_SOURCE_DIR}/src"
	"${CMAKE_SOURCE_DIR}/other/imgui"
	"${CMAKE_SOURCE_DIR}/other/serialport/include"
	${GLEW_INCLUDE_DIRS}
	${GLFW_INDLUCE_DIR}
	${MAR_GLFW_DIR}
	${STB_IMAGE_INCLUDE_DIR}
	${SPDLOG_INCLUDE_DIR}
	${MAR_MATHS_INCLUDE_DIR})

# MAREngine
message("Creating executable...")
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES} 
		${IMGUI_SOURCES} ${IMGUI_HEADERS} ${STB_IMAGE_H} ${STB_IMAGE_SRC}
		${SOURCE_MATH} ${HEADER_MATH} ${MAIN_MATH})
message("Linking libraries to MAREngine")
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)	
if(WIN32)
	set(MAR_OPENGL_IMPORT opengl32 glew32s glfw3)
	target_link_libraries ( ${PROJECT_NAME} PRIVATE ${MAR_OPENGL_IMPORT})
endif()

IF(MSVC)
	SET(VS_MULTITHREADED_DEBUG_DLL_IGNORE_LIBRARY_FLAGS
	   "/NODEFAULTLIB:libc.lib
	    /NODEFAULTLIB:libcmt.lib
	    /NODEFAULTLIB:msvcrt.lib
	    /NODEFAULTLIB:libcd.lib
	    /NODEFAULTLIB:libcmtd.lib")
	SET(VS_MULTITHREADED_RELEASE_DLL_IGNORE_LIBRARY_FLAGS
	   "/NODEFAULTLIB:libc.lib
	    /NODEFAULTLIB:libcmt.lib
	    /NODEFAULTLIB:libcd.lib
	    /NODEFAULTLIB:libcmtd.lib
	    /NODEFAULTLIB:msvcrtd.lib")

    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES
       LINK_FLAGS_DEBUG
       ${VS_MULTITHREADED_DEBUG_DLL_IGNORE_LIBRARY_FLAGS})
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES
        LINK_FLAGS_RELWITHDEBINFO
        ${VS_MULTITHREADED_RELEASE_DLL_IGNORE_LIBRARY_FLAGS})
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES
        LINK_FLAGS_RELEASE
        ${VS_MULTITHREADED_RELEASE_DLL_IGNORE_LIBRARY_FLAGS})
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES
        LINK_FLAGS_MINSIZEREL
        ${VS_MULTITHREADED_RELEASE_DLL_IGNORE_LIBRARY_FLAGS})
ENDIF(MSVC)

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