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


project(SandboxMAR CXX C)


set(SandboxMARSourcesPath ${SandboxMARPath}/src)
message(STATUS "Looking for all SandboxMAR files at ${SandboxMARPath} ...")
file(
    GLOB_RECURSE
    SandboxMARSources
	LIST_DIRECTORIES false
	${SandboxMARSourcesPath}/*.c
	${SandboxMARSourcesPath}/*.cpp
	${SandboxMARPath}/main.cpp
)

file(
    GLOB_RECURSE
    SandboxMARHeaders
	LIST_DIRECTORIES false
	${SandboxMARSourcesPath}/*.h
	${SandboxMARSourcesPath}/*.hpp
)

file(
    GLOB_RECURSE
    SandboxMARResources
    ${SandboxMARPath}/resources/shaders/*.glsl
    ${SandboxMARPath}/resources/shaders/*.spv
)

set(SandboxMARFiles ${SandboxMARSources} ${SandboxMARHeaders}) 
set(SandboxMARAllFiles ${SandboxMARFiles} ${SandboxMARResources})
set_property(GLOBAL PROPERTY SandboxMARAllFilesProperty ${SandboxMARAllFiles})

get_property(MAREngineIncludeDir GLOBAL PROPERTY MAREngineIncludeDirProperty)
get_property(MAREngineIncludeDirectories GLOBAL PROPERTY MAREngineIncludeDirectoriesProperty)
get_property(MAREngineIncludeLibraries GLOBAL PROPERTY MAREngineIncludeLibrariesProperty)
get_property(MAREngineLibrary GLOBAL PROPERTY MAREngineLibraryProperty)

message(STATUS " *** ${MAREngineIncludeDir}")
message(STATUS " *** ${MAREngineIncludeDirectories}")
message(STATUS " *** ${MAREngineIncludeLibraries}")
message(STATUS " *** ${MAREngineLibrary}")
include_directories(${MAREngineIncludeDir} ${MAREngineIncludeDirectories})
link_directories(${MAREngineIncludeLibraries})

message("Creating SandboxMAR executable...")
add_executable(SandboxMAR ${SandboxMARFiles} ${MAREngineIncludeDir})
target_compile_features(SandboxMAR PRIVATE cxx_std_17)
target_link_libraries(SandboxMAR PRIVATE ${MAREngineLibrary})


function(CopySubDirToBuildDirectory SubDirName)
    add_custom_command(
        TARGET SandboxMAR POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${SandboxMARPath}/${SubDirName} $<TARGET_FILE_DIR:SandboxMAR>/${SubDirName}
        COMMENT "Copying ${SandboxMARPath}/${SubDirName} => $<TARGET_FILE_DIR:SandboxMAR>/${SubDirName}"
    )
endfunction()

CopySubDirToBuildDirectory("resources")
CopySubDirToBuildDirectory("DefaultProject")


function(CopyFilesFromSubDirToBuildDir SubDirName)
	add_custom_command(
			TARGET SandboxMAR POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_directory ${SubDirName} $<TARGET_FILE_DIR:SandboxMAR>
			COMMENT "Copying ${SubDirName} => $<TARGET_FILE_DIR:SandboxMAR>"
	)
endfunction()

CopyFilesFromSubDirToBuildDir(${CMAKE_SOURCE_DIR}/MAREngine/3rd_party/python3.8.5/python-3.8.8-embed-win32/)


function(CopyFileToBuildDirectory Filename)
    add_custom_command(
        TARGET SandboxMAR POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${Filename} $<TARGET_FILE_DIR:SandboxMAR>
        COMMENT "Copying ${Filename} => $<TARGET_FILE_DIR:SandboxMAR>"
    )
endfunction()

CopyFileToBuildDirectory(${SandboxMARPath}/imgui.ini)
CopyFileToBuildDirectory(${SandboxMARPath}/desktop.ini)
