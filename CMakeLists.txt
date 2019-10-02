cmake_minimum_required(VERSION 3.8)

project(GLFW_TEST CXX)

include(CheckIncludeFile)
include(CheckIncludeFileCXX)
include(CheckIncludeFiles)
include(ExternalProject)
include(third_party/glfw.cmake)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

#add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(third_party)

set(SOURCE_LIST
    "src/main.cpp"
    "src/test.h"
)

add_executable(GLFW_TEST ${SOURCE_LIST})

# Create the source groups for source tree with root at CMAKE_CURRENT_SOURCE_DIR.
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_LIST})