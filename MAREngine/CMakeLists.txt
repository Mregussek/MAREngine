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

project(MAREngineLibrary CXX C)


set(MAREngineSourcesPath ${MAREnginePath}/src)
set(MAREngineIncludeDir ${MAREnginePath}/include)


message(STATUS "Looking for all MAREngine Core files at ${MAREngineSourcesPath} ...")
file(
    GLOB_RECURSE MAREngineSources
	LIST_DIRECTORIES false
	${MAREngineSourcesPath}/*.c
	${MAREngineSourcesPath}/*.cpp
)
file(
    GLOB_RECURSE MAREngineHeaders
	LIST_DIRECTORIES false
	${MAREngineSourcesPath}/*.h
    ${MAREngineSourcesPath}/*.inl
	${MAREngineSourcesPath}/*.hpp
)

set(
    MAREngineAllFiles
    ${MAREngineSources}
    ${MAREngineHeaders}
    ${MAREngineIncludeDir}/MAREngine/MAREngine.h
)
set_property(GLOBAL PROPERTY MAREngineAllFilesProperty ${MAREngineAllFiles})

add_subdirectory(${MAREnginePath}/3rd_party)

get_property(MAREngineIncludeLibraries GLOBAL PROPERTY MAREngineIncludeLibrariesProperty)
get_property(MAREngineIncludeDirectories GLOBAL PROPERTY MAREngineIncludeDirectoriesProperty)
get_property(MAREngine3rdPartyLibraries GLOBAL PROPERTY MAREngine3rdPartyLibrariesProperty)

message(STATUS "Setting all include directories...")
message(STATUS " *** ${MAREngineSourcesPath}")
message(STATUS " *** ${MAREngineIncludeDirectories}")
message(STATUS " *** ${MAREngineIncludeLibraries}")
message(STATUS " *** ${MAREngine3rdPartyLibraries}")

include_directories(${MAREngineSourcesPath} ${MAREngineIncludeDirectories})
link_directories(${MAREngineIncludeLibraries})

message(STATUS "Creating static MAREngine.lib library...")
add_library(MAREngineLibrary ${MAREngineAllFiles})
target_compile_features(MAREngineLibrary PRIVATE cxx_std_17)
target_link_libraries(MAREngineLibrary PRIVATE ${MAREngine3rdPartyLibraries})
