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


cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(MAREngine CXX C)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set_property(GLOBAL PROPERTY MAREnginePathProperty "${CMAKE_CURRENT_SOURCE_DIR}/MAREngine")
get_property(MAREnginePath GLOBAL PROPERTY MAREnginePathProperty)

message(STATUS "Adding subdirectory ${MAREnginePath} ...")
add_subdirectory(${MAREnginePath})

set_property(GLOBAL PROPERTY SandboxMARProperty "${CMAKE_CURRENT_SOURCE_DIR}/SandboxMAR")
get_property(SandboxMARPath GLOBAL PROPERTY SandboxMARProperty)
message(STATUS "Adding subdirectory ${SandboxMARPath} ...")
add_subdirectory(${SandboxMARPath})


get_property(MAREngineAllFiles GLOBAL PROPERTY MAREngineAllFilesProperty)
get_property(SandboxMARAllFiles GLOBAL PROPERTY SandboxMARAllFilesProperty)


if(MSVC)
	message(STATUS "Configuring MSVC...")

	function(MSVC_SetTargetPropertiesMAREngine ArgProjectName)
		set_target_properties(${ArgProjectName} PROPERTIES
			LINK_FLAGS_DEBUG ${VS_MULTITHREADED_DEBUG_DLL_IGNORE_LIBRARY_FLAGS}
 		)
 		set_target_properties(${ArgProjectName} PROPERTIES
			 LINK_FLAGS_RELWITHDEBINFO ${VS_MULTITHREADED_RELEASE_DLL_IGNORE_LIBRARY_FLAGS}
 		)
 		set_target_properties(${ArgProjectName} PROPERTIES
			 LINK_FLAGS_RELEASE ${VS_MULTITHREADED_RELEASE_DLL_IGNORE_LIBRARY_FLAGS}
 		)
 		set_target_properties(${ArgProjectName} PROPERTIES
			 LINK_FLAGS_MINSIZEREL ${VS_MULTITHREADED_RELEASE_DLL_IGNORE_LIBRARY_FLAGS}
 		)
	endfunction()

	# ignore dynamic linking
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
	    /NODEFAULTLIB:libcmtd.lib"
	)
	set(VS_MULTITHREADED_RELEASE_DLL_IGNORE_LIBRARY_FLAGS
	   "/NODEFAULTLIB:libc.lib
	    /NODEFAULTLIB:libcmt.lib
	    /NODEFAULTLIB:libcd.lib
	    /NODEFAULTLIB:libcmtd.lib
	    /NODEFAULTLIB:msvcrtd.lib"
	)

	MSVC_SetTargetPropertiesMAREngine(MAREngineLibrary)

	# Organize files in Visual Studio
	message(STATUS "Configuring source_group for ${MAREnginePath} ${MAREngineAllFiles}")
	source_group(TREE ${MAREnginePath} FILES ${MAREngineAllFiles})

	message(STATUS "Configuring source_group for ${SandboxMARPath} ${SandboxMARAllFiles}")
	source_group(TREE ${SandboxMARPath} FILES ${SandboxMARAllFiles})
endif()
