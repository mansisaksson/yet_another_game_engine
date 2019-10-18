cmake_minimum_required(VERSION 2.8)

SET(CMAKE_BUILD_TYPE Debug)

include(third_party/glfw.cmake)
include(third_party/glew.cmake)

set(PROJECT_NAME YETE)

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 14)

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

add_compile_definitions(ENGINE_CONTENT_DIR="${CMAKE_CURRENT_LIST_DIR}/content")

include_directories(third_party/nlohmannjson)
include_directories(third_party/glm)
include_directories(third_party/loaders)
include_directories(src)

add_subdirectory(third_party)
add_subdirectory(src)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_LIST}) # build source tree for some IDEs

add_executable(${PROJECT_NAME} ${SOURCE_LIST})
