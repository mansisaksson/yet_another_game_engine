cmake_minimum_required(VERSION 2.8)

set(CMAKE_BUILD_TYPE Debug)
set(PROJECT_NAME YETE)
set(CMAKE_CXX_STANDARD 17)

project(${PROJECT_NAME})

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
include(third_party/glew.cmake)
include(third_party/bullet3.cmake)

include_directories(third_party/nlohmannjson)
include_directories(src)

add_subdirectory(third_party)
add_subdirectory(src)

add_compile_definitions(ENGINE_CONTENT_DIR="${CMAKE_CURRENT_LIST_DIR}/content")

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_LIST}) # build source tree for some IDEs

add_executable(${PROJECT_NAME} ${SOURCE_LIST})

add_dependencies(${PROJECT_NAME} glfw)
add_dependencies(${PROJECT_NAME} glew)
add_dependencies(${PROJECT_NAME} bullet3)