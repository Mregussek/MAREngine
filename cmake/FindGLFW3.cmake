
# Set search paths
set( _glfw3_HEADER_SEARCH_DIRS
		"${CMAKE_SOURCE_DIR}/3rd_party"
		"${CMAKE_SOURCE_DIR}/3rd_party/GLFW"
		"${CMAKE_SOURCE_DIR}/3rd_party/GLFW/include"
)

set( _glfw3_LIB_SEARCH_DIRS
		"${CMAKE_SOURCE_DIR}/3rd_party"
		"${CMAKE_SOURCE_DIR}/3rd_party/GLFW"
		"${CMAKE_SOURCE_DIR}/3rd_party/GLFW/lib-vc2019"
)

# Check environment for root search directory
set( _glfw3_ENV_ROOT $ENV{GLFW3_ROOT} )
if( NOT GLFW3_ROOT AND _glfw3_ENV_ROOT )
	set(GLFW3_ROOT ${_glfw3_ENV_ROOT} )
endif()

# Put user specified location at beginning of search
if( GLFW3_ROOT )
	list( INSERT _glfw3_HEADER_SEARCH_DIRS 0 "${GLFW3_ROOT}/include" )
	list( INSERT _glfw3_LIB_SEARCH_DIRS 0 "${GLFW3_ROOT}/lib" )
endif()

# Search for the header
FIND_PATH(GLFW3_INCLUDE_DIR "GLFW/glfw3.h"
PATHS ${_glfw3_HEADER_SEARCH_DIRS} )

# Search for the library
FIND_LIBRARY(GLFW3_LIBRARY NAMES glfw3 glfw
PATHS ${_glfw3_LIB_SEARCH_DIRS} )
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GLFW3 DEFAULT_MSG
GLFW3_LIBRARY GLFW3_INCLUDE_DIR)