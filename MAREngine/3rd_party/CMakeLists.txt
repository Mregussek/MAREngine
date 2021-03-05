#***********************************************************************
# @internal @copyright
#
#  				MAREngine - open source 3D game engine
#
# Copyright (C) 2020-present Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
# All rights reserved.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.
#
#***********************************************************************


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/)


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

list(APPEND ${MAREngine3rdPartyLibraries} ${OPENGL_gl_LIBRARY})


message("Looking for: GLAD")
set(GLADDirectory ${CMAKE_CURRENT_SOURCE_DIR}/GLAD)
set(GLADIncludeDirectory ${CMAKE_CURRENT_SOURCE_DIR}/GLAD/include)
set(GLADSources ${GLADDirectory}/src/glad.c)
set(GLADHeaders ${GLADDirectory}/include/glad/glad.h ${GLADDirectory}/include/KHR/khrplatform.h)
set(GLADFiles ${GLADSources} ${GLADHeaders})
message(STATUS "GLADIncludeDirectory:= ${GLADIncludeDirectory}")

list(APPEND ${MAREngineIncludeDirectories} ${GLADIncludeDirectory})
list(APPEND ${MAREngineAllFiles} ${GLADFiles})


message("Looing for: SDL2")
set(SDLIncludeDirectory "${CMAKE_CURRENT_SOURCE_DIR}/SDL2-2.0.12/include")
file(
    GLOB_RECURSE SDLSources 
	${SDLIncludeDirectory}/*.c
	${SDLIncludeDirectory}/*.cpp
)
file(
    GLOB_RECURSE SDLHeaders 
	${SDLIncludeDirectory}/*.h
	${SDLIncludeDirectory}/*.hpp
)
set(SDLFiles ${SDLSources} ${SDLHeaders})
message(STATUS "SDLIncludeDirectory:= ${SDLIncludeDirectory}")

list(APPEND ${MAREngineAllFiles} ${SDLFiles})
list(APPEND ${MAREngine3rdPartyLibraries} SDL2 SDL2main SDL2test)
list(APPEND ${MAREngineIncludeDirectories} ${SDLIncludeDirectory})
list(APPEND ${MAREngineIncludeLibraries} ${CMAKE_CURRENT_SOURCE_DIR}/SDL2-2.0.12/lib/x86)


message("Looking for: GLFW")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
find_package(GLFW3 REQUIRED)
if(GLFW3_FOUND)
	message(STATUS "GLFW_INDLUCE_DIR:= ${GLFW3_INCLUDE_DIR}")
	message(STATUS "GLFW3_LIBRARY:= ${GLFW3_LIBRARY}")
	set(GLFWDirectory ${CMAKE_CURRENT_SOURCE_DIR}/GLFW/include)
	message(STATUS "GLFWDirectory:= ${GLFWDirectory}")
else()
	message(STATUS "Error: Cannot find GLFW3!")
endif()

list(APPEND ${MAREngine3rdPartyLibraries} ${GLFW3_LIBRARY})
list(APPEND ${MAREngineIncludeDirectories} ${GLFWDirectory})
list(APPEND ${MAREngineIncludeLibraries} ${CMAKE_CURRENT_SOURCE_DIR}/GLFW/lib-vc2019)


message("Looking for: stb_image")
set(STBImageIncludeDirectory ${CMAKE_CURRENT_SOURCE_DIR}/stb_image/include)
set(STBImageHeaders ${STBImageIncludeDirectory}/stb_image/stb_image.h)
set(STBImageSources ${STBImageIncludeDirectory}/stb_image/stb_image.cpp)
set(STBImageFiles ${STBImageSources} ${STBImageHeaders})
message(STATUS "STBImageIncludeDirectory:= ${STBImageIncludeDirectory}")

list(APPEND ${MAREngineAllFiles} ${STBImageFiles})
list(APPEND ${MAREngineIncludeDirectories} ${STBImageIncludeDirectory})


message("Looking for: Dear ImGui")
set(ImGuiIncludeDirectory ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
set(
    ImGuiSources
	${ImGuiIncludeDirectory}/backends/imgui_impl_glfw.cpp
    ${ImGuiIncludeDirectory}/backends/imgui_impl_opengl3.cpp
    ${ImGuiIncludeDirectory}/backends/imgui_impl_sdl.cpp
	${ImGuiIncludeDirectory}/imgui.cpp
	${ImGuiIncludeDirectory}/imgui_demo.cpp
	${ImGuiIncludeDirectory}/imgui_draw.cpp
	${ImGuiIncludeDirectory}/imgui_widgets.cpp
    ${ImGuiIncludeDirectory}/imgui_tables.cpp
)
set(
    ImGuiHeaders
	${ImGuiIncludeDirectory}/backends/imgui_impl_opengl3.h
    ${ImGuiIncludeDirectory}/backends/imgui_impl_glfw.h
    ${ImGuiIncludeDirectory}/backends/imgui_impl_sdl.h
	${ImGuiIncludeDirectory}/imgui.h
	${ImGuiIncludeDirectory}/imconfig.h
    ${ImGuiIncludeDirectory}/imgui_internal.h
    ${ImGuiIncludeDirectory}/imstb_rectpack.h
    ${ImGuiIncludeDirectory}/imstb_textedit.h
    ${ImGuiIncludeDirectory}/imstb_truetype.h
)
set(ImGuiFiles ${ImGuiSources} ${ImGuiHeaders})
message(STATUS "ImGuiIncludeDirectory:= ${ImGuiIncludeDirectory}")

list(APPEND ${MAREngineAllFiles} ${ImGuiFiles})
list(APPEND ${MAREngineIncludeDirectories} ${ImGuiIncludeDirectory})


message("Looking for: ImGuizmo")
set(ImGuizmoIncludeDirectory ${CMAKE_CURRENT_SOURCE_DIR}/ImGuizmo)
set(ImGuizmoHeaders ${ImGuizmoIncludeDirectory}/ImGuizmo.h)
set(ImGuizmoSources ${ImGuizmoIncludeDirectory}/ImGuizmo.cpp)
set(ImGuizmoFiles ${ImGuizmoSources} ${ImGuizmoHeaders})
message(STATUS "ImGuizmoIncludeDirectory:= ${ImGuizmoIncludeDirectory}")

list(APPEND ${MAREngineAllFiles} ${ImGuizmoFiles})
list(APPEND ${MAREngineIncludeDirectories} ${ImGuizmoIncludeDirectory})

