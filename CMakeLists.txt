cmake_minimum_required(VERSION 3.8)

project(GLFW_TEST CXX)

SET(CMAKE_BUILD_TYPE Debug)

macro (add_sources)
    file (RELATIVE_PATH _relPath "${PROJECT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")
    foreach (_src ${ARGN})
        if (_relPath)
            list (APPEND SOURCE_LIST "${_relPath}/${_src}")
        else()
            list (APPEND SOURCE_LIST "${_src}")
        endif()
    endforeach()
    if (_relPath)
        # propagate SRCS to parent directory
        set (SOURCE_LIST ${SOURCE_LIST} PARENT_SCOPE)
    endif()
endmacro()

include(third_party/glfw.cmake)
add_subdirectory(third_party)
add_subdirectory(src)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_LIST}) # build source tree for some IDEs

add_executable(GLFW_TEST ${SOURCE_LIST})
