message("")

message(STATUS "Configuring GLFW")

include(ExternalProject)

list(APPEND CMAKE_ARGS "-DBUILD_SHARED_LIBS:BOOL=ON")
list(APPEND CMAKE_ARGS "-DGLFW_BUILD_EXAMPLES:BOOL=FALSE")
list(APPEND CMAKE_ARGS "-DGLFW_BUILD_TESTS:BOOL=FALSE")
list(APPEND CMAKE_ARGS "-DGLFW_BUILD_DOCS:BOOL=FALSE")
list(APPEND CMAKE_ARGS "-DGLFW_INSTALL:BOOL=FALSE")
list(APPEND CMAKE_ARGS "-DGLFW_VULKAN_STATIC:BOOL=FALSE")

set(GLFW_PREFIX glfw)
set(GLFW_URL ${CMAKE_CURRENT_SOURCE_DIR}/third_party/glfw.zip)

ExternalProject_Add(${GLFW_PREFIX}
    PREFIX ${GLFW_PREFIX}
    URL ${GLFW_URL}
    CMAKE_ARGS ${CMAKE_ARGS}
    LOG_DOWNLOAD 1
    LOG_BUILD 1
)

# get the unpacked source/binary directory path
ExternalProject_Get_Property(${GLFW_PREFIX} SOURCE_DIR)
ExternalProject_Get_Property(${GLFW_PREFIX} BINARY_DIR)

# set the include directory variable and include it
set(GLFW_INCLUDE_DIRS ${SOURCE_DIR}/include)

# link the correct GLFW directory when the project is in Debug or Release mode
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	# in Debug mode
	set(GLFW_DEBUG_DIR ${BINARY_DIR}/src/Debug)

	set(GLFW_LIBS glfw3dll)
	set(GLFW_LIBRARY_DIRS ${GLFW_DEBUG_DIR})
	set(GLFW_BINARY_DIRS ${GLFW_DEBUG_DIR})
else (CMAKE_BUILD_TYPE STREQUAL "Debug")
	# in Release mode
	set(GLFW_RELEASE_DIR ${BINARY_DIR}/src/Release)

	set(GLFW_LIBS glfw3dll)
	set(GLFW_LIBRARY_DIRS ${GLFW_RELEASE_DIR})
	set(GLFW_BINARY_DIRS ${GLFW_RELEASE_DIR})
endif (CMAKE_BUILD_TYPE STREQUAL "Debug")

message(STATUS "GLFW Include directory ${GLFW_INCLUDE_DIRS}")
message(STATUS "GLFW Library directory ${GLFW_LIBRARY_DIRS}")
message(STATUS "GLFW Binary directory ${GLFW_BINARY_DIRS}")

# set include dirs
include_directories(${GLFW_INCLUDE_DIRS})

# link libraries
link_directories(${GLFW_LIBRARY_DIRS})
link_libraries(${GLFW_LIBS})

# install .dll/.so
set(GLFW_INSTALL_DIR ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
if (UNIX)
	install(DIRECTORY ${GLFW_BINARY_DIRS}/ DESTINATION ${GLFW_INSTALL_DIR} USE_SOURCE_PERMISSIONS FILES_MATCHING PATTERN "*.so*")
else (UNIX)
	install(DIRECTORY ${GLFW_BINARY_DIRS}/ DESTINATION ${GLFW_INSTALL_DIR} FILES_MATCHING PATTERN "*.dll*")
endif (UNIX)

message("")