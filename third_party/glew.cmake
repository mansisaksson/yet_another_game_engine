message("")

message(STATUS "Configuring GLEW")

include(ExternalProject)

find_package(OpenGL REQUIRED)

# list(APPEND CMAKE_ARGS "-DBUILD_UTILS:BOOL=ON")
# list(APPEND CMAKE_ARGS "-DGLEW_REGAL:BOOL=FALSE")
# list(APPEND CMAKE_ARGS "-DGLEW_OSMESA:BOOL=FALSE")

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

# set the include directory variable and include it
set(GLEW_INCLUDE_DIRS ${SOURCE_DIR}/include)

# link the correct GLEW directory when the project is in Debug or Release mode
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	# in Debug mode
	set(GLEW_LIBS glew32d libglew32d opengl32)
    set(GLEW_LIBRARY_DIRS ${BINARY_DIR}/lib/Debug)
    set(GLEW_BINARY_DIRS ${BINARY_DIR}/bin/Debug)
else (CMAKE_BUILD_TYPE STREQUAL "Debug")
	# in Release mode
	set(GLEW_LIBS glew32 libglew32 opengl32)
    set(GLEW_LIBRARY_DIRS ${BINARY_DIR}/lib/Release)
    set(GLEW_BINARY_DIRS ${BINARY_DIR}/bin/Release)
endif (CMAKE_BUILD_TYPE STREQUAL "Debug")

message(STATUS "GLEW Include directory ${GLFW_INCLUDE_DIRS}")
message(STATUS "GLEW Library directory ${GLFW_LIBRARY_DIRS}")
message(STATUS "GLEW Binary directory ${GLEW_BINARY_DIRS}")

# set include dirs
include_directories(${GLEW_INCLUDE_DIRS})

# link libraries

link_directories(${GLEW_LIBRARY_DIRS})
link_libraries(${GLEW_LIBS})

# install .dll/.so
set(GLEW_INSTALL_DIR ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
if (UNIX)
	install(DIRECTORY ${GLEW_BINARY_DIRS}/ DESTINATION ${GLEW_INSTALL_DIR} USE_SOURCE_PERMISSIONS FILES_MATCHING PATTERN "*.so*")
else (UNIX)
	install(DIRECTORY ${GLEW_BINARY_DIRS}/ DESTINATION ${GLEW_INSTALL_DIR} FILES_MATCHING PATTERN "*.dll*")
endif (UNIX)

message("")