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

project(MAREngineLibrary)

# path to MAREngine sources
set(MAREnginePath ${CMAKE_CURRENT_SOURCE_DIR})
# List name with all MAREngine files, that must be compiled
set(MAREngineAllFiles MAREngineAllFilesList)
# List name with all 3rd_party libraries directories
set(MAREngineIncludeLibraries MAREngineIncludeLibsList)
# List name with all 3rd_party sources directories
set(MAREngineIncludeDirectories MAREngineIncludeDirsList)
# List name with all 3rd_party compiled libraries (.lib files)
set(MAREngine3rdPartyLibraries MAREngine3rdPartyLibsList)


file(
    GLOB_RECURSE MAREngineSources
	LIST_DIRECTORIES false
	${MAREnginePath}/src/*.c
	${MAREnginePath}/src/*.cpp
)
file(
    GLOB_RECURSE MAREngineHeaders
	LIST_DIRECTORIES false
	${MAREnginePath}/src/*.h
    ${MAREnginePath}/src/*.inl
	${MAREnginePath}/src/*.hpp
)

list(APPEND ${MAREngineAllFiles} ${MAREngineSources} ${MAREngineHeaders})

list(APPEND ${MAREngineIncludeLibraries} ".")
list(APPEND ${MAREngineIncludeDirectories} ".")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rd_party)