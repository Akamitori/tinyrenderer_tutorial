cmake_minimum_required(VERSION 3.30)
project(graphics_tiny_renderer)

set(CMAKE_CXX_STANDARD 20)

add_executable(graphics_tiny_renderer main.cpp tgaimage.cpp)
