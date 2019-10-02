cmake_minimum_required(VERSION 3.8)

project(GLFW_TEST CXX)

SET(CMAKE_BUILD_TYPE Debug)

include(third_party/glfw.cmake)

add_subdirectory(third_party)
add_subdirectory(src)

set(SOURCE_LIST
    "src/main.cpp"
    "src/test.h"
)

add_executable(GLFW_TEST ${SOURCE_LIST})

# Create the source groups for source tree with root at CMAKE_CURRENT_SOURCE_DIR.
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_LIST})