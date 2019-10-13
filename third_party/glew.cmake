message("\n")
message(STATUS "Configuring GLEW")

include(CheckIncludeFileCXX)
include(ExternalProject)

# list(APPEND CMAKE_ARGS "-DBUILD_SHARED_LIBS:BOOL=ON")
# list(APPEND CMAKE_ARGS "-DGLFW_BUILD_EXAMPLES:BOOL=FALSE")
# list(APPEND CMAKE_ARGS "-DGLFW_BUILD_TESTS:BOOL=FALSE")
# list(APPEND CMAKE_ARGS "-DGLFW_BUILD_DOCS:BOOL=FALSE")
# list(APPEND CMAKE_ARGS "-DGLFW_INSTALL:BOOL=FALSE")
# list(APPEND CMAKE_ARGS "-DGLFW_VULKAN_STATIC:BOOL=FALSE")

set(GLEW_PREFIX glew)
set(GLEW_URL ${CMAKE_CURRENT_SOURCE_DIR}/third_party/glew.zip)


ExternalProject_Add ( ${GLEW_PREFIX}_download
    PREFIX ${GLEW_PREFIX}
    SOURCE_DIR "${GLEW_PREFIX}/src/glew" # Explicitely set download dir
    URL ${GLEW_URL}
    CONFIGURE_COMMAND "" # Disable all other steps
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)

ExternalProject_Add ( ${GLEW_PREFIX}
    PREFIX ${GLEW_PREFIX}
    DOWNLOAD_COMMAND "" # Disable download step
    SOURCE_DIR "${GLEW_PREFIX}/src/glew/build/cmake" # Source dir for configuration
    CMAKE_CACHE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
    LOG_DOWNLOAD 1
    LOG_BUILD 1
)

# ExternalProject_Add(${GLEW_PREFIX}
#     PREFIX ${GLEW_PREFIX}
#     URL ${GLEW_URL}
#     CMAKE_ARGS ${CMAKE_ARGS}
#     #CONFIGURE_COMMAND ${CMAKE_COMMAND} -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR><SOURCE_DIR>/build/cmake/ # Tell CMake to use subdirectory as source.
#     SOURCE_DIR "build/cmake"
#     LOG_DOWNLOAD 1
#     LOG_BUILD 1
# )

# get the unpacked source/binary directory path
ExternalProject_Get_Property(${GLEW_PREFIX} SOURCE_DIR)
ExternalProject_Get_Property(${GLEW_PREFIX} BINARY_DIR)

set (SOURCE_DIR ${SOURCE_DIR}/../..) # need to adjust for our CMakeLists.txt being in {SOURCE_DIR}/build/cmake

message(STATUS "Source directory of ${GLEW_PREFIX} ${SOURCE_DIR}")
message(STATUS "Binary directory of ${GLEW_PREFIX} ${BINARY_DIR}")

# Set separate directories for building in Debug or Release mode
set(GLEW_DEBUG_DIR ${BINARY_DIR}/lib/Debug)
set(GLEW_RELEASE_DIR ${BINARY_DIR}/lib/Release)
message(STATUS "GLEW Debug directory ${GLEW_DEBUG_DIR}")
message(STATUS "GLEW Release directory ${GLEW_RELEASE_DIR}")

# set the include directory variable and include it
set(GLEW_INCLUDE_DIRS ${SOURCE_DIR}/include)
message(STATUS "GLEW Include directory ${GLEW_INCLUDE_DIRS}")

include_directories(${GLEW_INCLUDE_DIRS})

# verify that the HAVE_GLEW header files can be included
set(CMAKE_REQUIRED_INCLUDES_SAVE ${CMAKE_REQUIRED_INCLUDES})
set(CMAKE_REQUIRED_INCLUDES ${CMAKE_REQUIRED_INCLUDES} ${GLEW_INCLUDE_DIRS})
check_include_file_cxx("GL/glew.h" HAVE_GLEW)
set(CMAKE_REQUIRED_INCLUDES ${CMAKE_REQUIRED_INCLUDES_SAVE})
if (NOT HAVE_GLEW)
    message(STATUS "Did not build GLEW correctly as cannot find GL/glew.h")
    set(HAVE_GLEW 1)
endif (NOT HAVE_GLEW)


# link the correct GLEW directory when the project is in Debug or Release mode
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	# in Debug mode
	set(GLEW_LIBS glew32d)
	set(GLEW_LIBRARY_DIRS ${GLEW_DEBUG_DIR})
else (CMAKE_BUILD_TYPE STREQUAL "Debug")
	# in Release mode
	set(GLEW_LIBS glew32)
	set(GLEW_LIBRARY_DIRS ${GLEW_RELEASE_DIR})
endif (CMAKE_BUILD_TYPE STREQUAL "Debug")

link_directories(${GLEW_LIBRARY_DIRS})
link_libraries(${GLEW_LIBS})

set(GLEW_INSTALL_DIR ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})

if (UNIX)
	install(DIRECTORY ${GLEW_LIBRARY_DIRS}/ DESTINATION ${GLEW_INSTALL_DIR} USE_SOURCE_PERMISSIONS FILES_MATCHING PATTERN "*.so*")
else (UNIX)
	install(DIRECTORY ${GLEW_LIBRARY_DIRS}/ DESTINATION ${GLEW_INSTALL_DIR} FILES_MATCHING PATTERN "*.dll*")
endif (UNIX)
