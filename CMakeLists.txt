cmake_minimum_required(VERSION 3.10)
project(Chess)

add_executable(Chess main.cpp
        piece_moves.cpp
        piece_moves.h)
target_link_libraries(Chess SDL2 SDL2_image SDL2_gfx)

file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})