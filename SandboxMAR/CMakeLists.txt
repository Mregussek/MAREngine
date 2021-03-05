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

include_directories(${MAREngineIncludeDir})

message("Creating SandboxMAR executable...")
add_executable(SandboxMAR ${SandboxMARFiles})
target_compile_features(SandboxMAR PRIVATE cxx_std_17)
target_link_libraries(SandboxMAR PRIVATE MAREngineLibrary)