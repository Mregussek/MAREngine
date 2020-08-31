#
# Mateusz Rzeczyca
# Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
#

cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(MARMaths)

set(MAR_MATH_LIB ${PROJECT_NAME})

file(GLOB_RECURSE MAR_MATH_LIB_SOURCE_FILES 
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)
	
file(GLOB_RECURSE MAR_MATH_LIB_HEADER_FILES 
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp
)
	
set(MAR_MATH_INCLUDE_FILES ${MAR_MATH_LIB_HEADER_FILES} ${MAR_MATH_LIB_SOURCE_FILES} ${MAIN_INCLUDE_FILE})
set(MAIN_INCLUDE_FILE ${CMAKE_CURRENT_SOURCE_DIR}/include/MARMaths.h)
	
include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_library(${MAR_MATH_LIB} STATIC ${MAR_MATH_INCLUDE_FILES})
target_compile_features(${MAR_MATH_LIB} PRIVATE cxx_std_17)