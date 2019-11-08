message(STATUS "Configuring GLFW")

include(ExternalProject)

set(GLFW_PREFIX glfw)

ExternalProject_Add(glfw
    PREFIX ${GLFW_PREFIX}
    URL ${CMAKE_CURRENT_SOURCE_DIR}/third_party/glfw.zip
    LOG_DOWNLOAD 1
	LOG_BUILD 1
	CMAKE_ARGS
		-DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
		-DBUILD_SHARED_LIBS:BOOL=ON
		-DGLFW_BUILD_EXAMPLES:BOOL=FALSE
		-DGLFW_BUILD_TESTS:BOOL=FALSE
		-DGLFW_BUILD_DOCS:BOOL=FALSE
		-DGLFW_INSTALL:BOOL=ON
		-DGLFW_VULKAN_STATIC:BOOL=FALSE
)

# get the unpacked source/binary directory path
ExternalProject_Get_Property(glfw INSTALL_DIR)

# set the include/lib/bin directory variables
set(GLFW_INCLUDE_DIRS ${INSTALL_DIR}/include)
set(GLFW_LIBRARY_DIRS ${INSTALL_DIR}/lib)
set(GLFW_BINARY_DIRS ${INSTALL_DIR}/bin)

# link the correct GLFW directory when the project is in Debug or Release mode
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(GLFW_LIBS glfw3dll)
else (CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(GLFW_LIBS glfw3dll)
endif (CMAKE_BUILD_TYPE STREQUAL "Debug")

# set include dirs
include_directories(${GLFW_INCLUDE_DIRS})

# link libraries
link_directories(${GLFW_LIBRARY_DIRS})
link_libraries(${GLFW_LIBS})

# install .dll/.so
set(GLFW_INSTALL_DIR ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})

if (UNIX)
	set(BIN_FILE_PATTERN "*.so*")
else (UNIX)
	set(BIN_FILE_PATTERN "*.dll*")
endif (UNIX)

install(DIRECTORY ${GLFW_BINARY_DIRS}/ DESTINATION ${GLFW_INSTALL_DIR} USE_SOURCE_PERMISSIONS FILES_MATCHING PATTERN ${BIN_FILE_PATTERN})