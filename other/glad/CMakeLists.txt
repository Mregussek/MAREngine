#
#	Mateusz Rzeczyca
#	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
#

include_directories(
    "${CMAKE_SOURCE_DIR}/3rd_party/glad/include"
)

add_library(
    GLAD 
    "${CMAKE_SOURCE_DIR}/other/glad/glad.c"
)

