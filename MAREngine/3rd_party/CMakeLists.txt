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

set(MAREngine3rdPartyLibraries ${MAREngine3rdPartyLibraries} ${OPENGL_gl_LIBRARY})


message("Looking for: GLAD")
set(GLADDirectory ${CMAKE_CURRENT_SOURCE_DIR}/GLAD)
set(GLADIncludeDirectory ${GLADDirectory}/include)
set(GLADSources ${GLADDirectory}/src/glad.c)
set(GLADHeaders ${GLADIncludeDirectory}/glad/glad.h ${GLADIncludeDirectory}/KHR/khrplatform.h)
set(GLADFiles ${GLADSources} ${GLADHeaders})
message(STATUS "GLADIncludeDirectory:= ${GLADIncludeDirectory}")

set(MAREngineIncludeDirectories ${MAREngineIncludeDirectories} ${GLADIncludeDirectory})
set(MAREngineAllFiles ${MAREngineAllFiles} ${GLADFiles})


message("Looking for: SDL2")
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

set(MAREngineAllFiles ${MAREngineAllFiles} ${SDLFiles})
set(MAREngine3rdPartyLibraries ${MAREngine3rdPartyLibraries} SDL2 SDL2main SDL2test)
set(MAREngineIncludeDirectories ${MAREngineIncludeDirectories} ${SDLIncludeDirectory})
set(MAREngineIncludeLibraries ${MAREngineIncludeLibraries} ${CMAKE_CURRENT_SOURCE_DIR}/SDL2-2.0.12/lib/x86)


message("Looking for: GLFW")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
find_package(GLFW3 REQUIRED)
if(GLFW3_FOUND)
	message(STATUS "GLFW3_INCLUDE_DIR:= ${GLFW3_INCLUDE_DIR}")
	message(STATUS "GLFW3_LIBRARY:= ${GLFW3_LIBRARY}")
	set(GLFWDirectory ${CMAKE_CURRENT_SOURCE_DIR}/GLFW/include)
	message(STATUS "GLFWDirectory:= ${GLFWDirectory}")
else()
	message(STATUS "Error: Cannot find GLFW3!")
endif()

set(MAREngine3rdPartyLibraries ${MAREngine3rdPartyLibraries} ${GLFW3_LIBRARY})
set(MAREngineIncludeDirectories ${MAREngineIncludeDirectories} ${GLFWDirectory})
set(MAREngineIncludeLibraries ${MAREngineIncludeLibraries} ${CMAKE_CURRENT_SOURCE_DIR}/GLFW/lib-vc2019)


message("Looking for: stb_image")
set(STBImageIncludeDirectory ${CMAKE_CURRENT_SOURCE_DIR}/stb_image/include)
set(STBImageHeaders ${STBImageIncludeDirectory}/stb_image/stb_image.h)
set(STBImageSources ${STBImageIncludeDirectory}/stb_image/stb_image.cpp)
set(STBImageFiles ${STBImageSources} ${STBImageHeaders})
message(STATUS "STBImageIncludeDirectory:= ${STBImageIncludeDirectory}")

set(MAREngineAllFiles ${MAREngineAllFiles} ${STBImageFiles})
set(MAREngineIncludeDirectories ${MAREngineIncludeDirectories} ${STBImageIncludeDirectory})


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

set(MAREngineAllFiles ${MAREngineAllFiles} ${ImGuiFiles})
set(MAREngineIncludeDirectories ${MAREngineIncludeDirectories} ${ImGuiIncludeDirectory})


message("Looking for: ImGuizmo")
set(ImGuizmoIncludeDirectory ${CMAKE_CURRENT_SOURCE_DIR}/ImGuizmo)
set(ImGuizmoHeaders ${ImGuizmoIncludeDirectory}/ImGuizmo.h)
set(ImGuizmoSources ${ImGuizmoIncludeDirectory}/ImGuizmo.cpp)
set(ImGuizmoFiles ${ImGuizmoSources} ${ImGuizmoHeaders})
message(STATUS "ImGuizmoIncludeDirectory:= ${ImGuizmoIncludeDirectory}")

set(MAREngineAllFiles ${MAREngineAllFiles} ${ImGuizmoFiles})
set(MAREngineIncludeDirectories ${MAREngineIncludeDirectories} ${ImGuizmoIncludeDirectory})


message("Looking for: ImGuiColorTextEdit")
set(ImGuiColorTextEditIncludeDirectory ${CMAKE_CURRENT_SOURCE_DIR}/ImGuiColorTextEdit)
set(
    ImGuiColorTextEditFiles
    ${ImGuiColorTextEditIncludeDirectory}/TextEditor.cpp
    ${ImGuiColorTextEditIncludeDirectory}/TextEditor.h
)
message(STATUS "ImGuiColorTextEditIncludeDirectory:= ${ImGuiColorTextEditIncludeDirectory}")

set(MAREngineAllFiles ${MAREngineAllFiles} ${ImGuiColorTextEditFiles})
set(MAREngineIncludeDirectories ${MAREngineIncludeDirectories} ${ImGuiColorTextEditIncludeDirectory})


message("Looking for: ImGuiAddons")
set(ImGuiAddonsIncludeDirectory "${CMAKE_CURRENT_SOURCE_DIR}/ImGui-Addons")
set(ImGuiAddonsFiles
	${ImGuiAddonsIncludeDirectory}/FileBrowser/ImGuiFileBrowser.h
	${ImGuiAddonsIncludeDirectory}/FileBrowser/ImGuiFileBrowser.cpp
	${ImGuiAddonsIncludeDirectory}/FileBrowser/Dirent/dirent.h
)
message(STATUS "ImGuiAddonsIncludeDirectory:= ${ImGuiAddonsIncludeDirectory}")

set(MAREngineAllFiles ${MAREngineAllFiles} ${ImGuiAddonsFiles})
set(MAREngineIncludeDirectories ${MAREngineIncludeDirectories} ${ImGuiAddonsIncludeDirectory})


message("Looking for: spdlog")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/spdlog)
set(spdlogIncludeDirectory ${CMAKE_CURRENT_SOURCE_DIR}/spdlog/include)
message(STATUS "spdlogIncludeDirectory:= ${spdlogIncludeDirectory}")

set(MAREngineIncludeDirectories ${MAREngineIncludeDirectories} ${spdlogIncludeDirectory})


message("Looking for: entt")
set(enttIncludeDirectory ${CMAKE_CURRENT_SOURCE_DIR}/entt/include)
set(enttFiles ${enttIncludeDirectory}/entt/entt.hpp)
message(STATUS "enttIncludeDirectory:= ${enttIncludeDirectory}")

set(MAREngineAllFiles ${MAREngineAllFiles} ${enttFiles})
set(MAREngineIncludeDirectories ${MAREngineIncludeDirectories} ${enttIncludeDirectory})


message("Looking for: json")
set(jsonIncludeDirectory ${CMAKE_CURRENT_SOURCE_DIR}/json/include)
set(jsonFiles ${jsonIncludeDirectory}/json.hpp)
message(STATUS "jsonIncludeDirectory:= ${jsonIncludeDirectory}")

set(MAREngineAllFiles ${MAREngineAllFiles} ${jsonFiles})
set(MAREngineIncludeDirectories ${MAREngineIncludeDirectories} ${jsonIncludeDirectory})


message("Looking for: python 3.8.5")
set(Python385IncludeDirectory ${CMAKE_CURRENT_SOURCE_DIR}/python3.8.5/include)
set(Python385IncludeLibraries ${CMAKE_CURRENT_SOURCE_DIR}/python3.8.5/libs)
set(
	PYTHON_INCLUDE_DIR
	${Python385IncludeDirectory}
	${CMAKE_CURRENT_SOURCE_DIR}/python3.8.5/python-3.8.8-embed-win32
)
set(
	PYTHON_LIBRARY
	${Python385IncludeLibraries}
	${CMAKE_CURRENT_SOURCE_DIR}/python3.8.5/python-3.8.8-embed-win32
)
set(PYTHON_EXECUTABLE ${CMAKE_CURRENT_SOURCE_DIR}/python3.8.5/python-3.8.8-embed-win32/python.exe)
message(STATUS "Python385IncludeDirectory:= ${Python385IncludeDirectory}")
message(STATUS "Python385IncludeLibraries:= ${Python385IncludeLibraries}")

set(MAREngine3rdPartyLibraries ${MAREngine3rdPartyLibraries} python38)
set(MAREngineIncludeDirectories ${MAREngineIncludeDirectories} ${Python385IncludeDirectory})
set(MAREngineIncludeLibraries ${MAREngineIncludeLibraries} ${Python385IncludeLibraries})


message("Looking for: pybind11")
set(PYBIND11_PYTHON_VERSION "3.8")
set(PYBIND11_NOPYTHON True)
set(PYBIND11_FINDPYTHON False)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/pybind11)
set(pybind11IncludeDirectory ${CMAKE_CURRENT_SOURCE_DIR}/pybind11/include)
message(STATUS "pybind11IncludeDirectory:= ${pybind11IncludeDirectory}")

set(MAREngine3rdPartyLibraries ${MAREngine3rdPartyLibraries} pybind11::embed)
set(MAREngineIncludeDirectories ${MAREngineIncludeDirectories} ${pybind11IncludeDirectory})


message("Looking for: MARMaths")
set(MARMathsIncludeDirectory ${CMAKE_CURRENT_SOURCE_DIR}/MARMaths/include)
set(MARMathsSourcesDir ${CMAKE_CURRENT_SOURCE_DIR}/MARMaths/src)
file(
    GLOB_RECURSE
    MARMathsSources
	${MARMathsSourcesDir}/*.c
	${MARMathsSourcesDir}/*.cpp
)
file(
    GLOB_RECURSE
    MARMathsHeaders
	${MARMathsSourcesDir}/*.h
	${MARMathsSourcesDir}/*.hpp
)
set(MARMathsFiles ${MARMathsIncludeDirectory}/MARMaths.h ${MARMathsSources} ${MARMathsHeaders})
message(STATUS "MARMathsIncludeDirectory:= ${MARMathsIncludeDirectory}")

set(MAREngineAllFiles ${MAREngineAllFiles} ${MARMathsFiles})
set(MAREngineIncludeDirectories ${MAREngineIncludeDirectories} ${MARMathsIncludeDirectory})



set_property(GLOBAL PROPERTY MAREngineIncludeLibrariesProperty ${MAREngineIncludeLibraries})
set_property(GLOBAL PROPERTY MAREngineIncludeDirectoriesProperty ${MAREngineIncludeDirectories})
set_property(GLOBAL PROPERTY MAREngine3rdPartyLibrariesProperty ${MAREngine3rdPartyLibraries})
set_property(GLOBAL PROPERTY MAREngineAllFilesProperty ${MAREngineAllFiles})
