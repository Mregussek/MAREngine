#
#	Mateusz Rzeczyca
#	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
#

add_subdirectory(glad)
add_subdirectory(imgui)
add_subdirectory(spdlog)

add_library(
    STB_IMAGE 
    "${CMAKE_SOURCE_DIR}/other/stb_image/include/stb_image/stb_image.cpp"
)