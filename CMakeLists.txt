cmake_minimum_required(VERSION 3.30)
project(graphics_tiny_renderer)

set(CMAKE_CXX_STANDARD 20)

add_executable(graphics_tiny_renderer main.cpp Vector3D.h Vector2D.h tgaimage.cpp triangle_drawing.cpp Model.cpp)

set(MODEL_SOURCE_DIR "${CMAKE_SOURCE_DIR}/data")
set(MODEL_DEST_DIR "${CMAKE_BINARY_DIR}/data")

message("SOME MESSAGE ${MODEL_SOURCE_DIR}")
message("SOME MESSAGE ${MODEL_DEST_DIR}")

# Copy the data to the build directory
add_custom_target(copy_data ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${MODEL_SOURCE_DIR}" "${MODEL_DEST_DIR}"
        COMMENT "Copying shader programs to build directory"
)

add_dependencies(graphics_tiny_renderer copy_data)