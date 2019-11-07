message("")

message(STATUS "Configuring Bullet3")

include(ExternalProject)

SET(bullet_CMAKE_ARGS 
    -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
    -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    -DCMAKE_OSX_ARCHITECTURES=${CMAKE_OSX_ARCHITECTURES}
    -DCMAKE_DEBUG_POSTFIX=_d
    
    -DBUILD_SHARED_LIBS=ON
    -DINSTALL_LIBS=ON

    -DINSTALL_CMAKE_FILES=OFF
    -DUSE_SOFT_BODY_MULTI_BODY_DYNAMICS_WORLD=ON
    -DUSE_DOUBLE_PRECISION=OFF
    -DUSE_GRAPHICAL_BENCHMARK=OFF
    -DBULLET2_MULTITHREADING=OFF
    -DUSE_MSVC_INCREMENTAL_LINKING=OFF
    -DUSE_CUSTOM_VECTOR_MATH=OFF
    -DBUILD_CPU_DEMOS=OFF
    -DUSE_GLUT=OFF
    -DBUILD_PYBULLET=OFF
    -DBUILD_ENET=OFF
    -DBUILD_CLSOCKET=OFF
    -DBUILD_OPENGL3_DEMOS=OFF
    -DBUILD_BULLET2_DEMOS=OFF
    -DBUILD_EXTRAS=OFF
    -DBUILD_UNIT_TESTS=OFF
)

set(BULLET_PREFIX bullet3)
set(BULLET_URL ${CMAKE_CURRENT_SOURCE_DIR}/third_party/bullet3.zip)

ExternalProject_Add(bullet3
    PREFIX ${BULLET_PREFIX}
    URL ${BULLET_URL}
	CMAKE_ARGS ${bullet_CMAKE_ARGS}
    LOG_DOWNLOAD 1
    LOG_BUILD 1
)

# get the unpacked source/binary directory path
ExternalProject_Get_Property(${BULLET_PREFIX} SOURCE_DIR)
ExternalProject_Get_Property(${BULLET_PREFIX} BINARY_DIR)

# set the include directory variable and include it
set(BULLET_INCLUDE_DIRS ${SOURCE_DIR}/include)

# link the correct BULLET directory when the project is in Debug or Release mode
#if (CMAKE_BUILD_TYPE STREQUAL "Debug")
#	# in Debug mode
#	set(BULLET_DEBUG_DIR ${BINARY_DIR}/src/Debug)
#
#	set(BULLET_LIBS glfw3dll)
#	set(BULLET_LIBRARY_DIRS ${BULLET_DEBUG_DIR})
#	set(BULLET_BINARY_DIRS ${BULLET_DEBUG_DIR})
#else (CMAKE_BUILD_TYPE STREQUAL "Debug")
#	# in Release mode
#	set(BULLET_RELEASE_DIR ${BINARY_DIR}/src/Release)
#
#	set(BULLET_LIBS glfw3dll)
#	set(BULLET_LIBRARY_DIRS ${BULLET_RELEASE_DIR})
#	set(BULLET_BINARY_DIRS ${BULLET_RELEASE_DIR})
#endif (CMAKE_BUILD_TYPE STREQUAL "Debug")

#message(STATUS "BULLET Include directory ${BULLET_INCLUDE_DIRS}")
#message(STATUS "BULLET Library directory ${BULLET_LIBRARY_DIRS}")
#message(STATUS "BULLET Binary directory ${BULLET_BINARY_DIRS}")

# set include dirs
#include_directories(${BULLET_INCLUDE_DIRS})

# link libraries
#link_directories(${BULLET_LIBRARY_DIRS})
#link_libraries(${BULLET_LIBS})

# install .dll/.so
#set(BULLET_INSTALL_DIR ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
#if (UNIX)
#	install(DIRECTORY ${BULLET_BINARY_DIRS}/ DESTINATION ${BULLET_INSTALL_DIR} USE_SOURCE_PERMISSIONS FILES_MATCHING PATTERN "*.so*")
#else (UNIX)
#	install(DIRECTORY ${BULLET_BINARY_DIRS}/ DESTINATION ${BULLET_INSTALL_DIR} FILES_MATCHING PATTERN "*.dll*")
#endif (UNIX)

message("")