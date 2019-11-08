message(STATUS "Configuring Bullet3")

include(ExternalProject)

set(BULLET_PREFIX bullet3)
message(${CMAKE_CURRENT_BINARY_DIR})

# todo: test this on unix
if (UNIX)
    set(args 
        -DBUILD_SHARED_LIBS=ON
        -DINSTALL_LIBS=ON
        -DINSTALL_CMAKE_FILES=OFF
    )
else (UNIX)
    set(args 
        -DUSE_MSVC_INCREMENTAL_LINKING=ON
        -DUSE_MSVC_RUNTIME_LIBRARY_DLL=ON
        -DBUILD_SHARED_LIBS=OFF
        -DINSTALL_LIBS=ON
        -DINSTALL_CMAKE_FILES=OFF
    )
endif (UNIX)

ExternalProject_Add(bullet3
    PREFIX ${BULLET_PREFIX}
    URL ${CMAKE_CURRENT_SOURCE_DIR}/third_party/bullet3.zip
    LOG_DOWNLOAD 1
    LOG_BUILD 1
    CMAKE_ARGS
        ${args}
        -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
        
        -DUSE_SOFT_BODY_MULTI_BODY_DYNAMICS_WORLD=ON
        -DUSE_DOUBLE_PRECISION=OFF
        -DUSE_GRAPHICAL_BENCHMARK=OFF
        -DBULLET2_MULTITHREADING=OFF
        -DUSE_CUSTOM_VECTOR_MATH=OFF
        -DUSE_GLUT=OFF

        -DBUILD_CPU_DEMOS=OFF
        -DBUILD_PYBULLET=OFF
        -DBUILD_ENET=OFF
        -DBUILD_CLSOCKET=OFF
        -DBUILD_OPENGL3_DEMOS=OFF
        -DBUILD_BULLET2_DEMOS=OFF
        -DBUILD_EXTRAS=OFF
        -DBUILD_UNIT_TESTS=OFF
)

# get the unpacked source/binary directory path
ExternalProject_Get_Property(bullet3 INSTALL_DIR)

# set the include/lib/bin directory variables
set(BULLET_INCLUDE_DIRS ${INSTALL_DIR}/include)
set(BULLET_LIBRARY_DIRS ${INSTALL_DIR}/lib)
set(BULLET_BINARY_DIRS ${INSTALL_DIR}/bin)

# link the correct Bullet directory when the project is in Debug or Release mode
if (CMAKE_BUILD_TYPE STREQUAL "Debug") # in Debug mode
    set(BULLET_LIBS 
        Bullet2FileLoader_Debug
        Bullet3Collision_Debug
        Bullet3Common_Debug
        Bullet3Dynamics_Debug
        Bullet3Geometry_Debug
        Bullet3OpenCL_clew_Debug
        BulletCollision_Debug
        BulletDynamics_Debug
        BulletInverseDynamics_Debug
        BulletSoftBody_Debug
        LinearMath_Debug
    )
else (CMAKE_BUILD_TYPE STREQUAL "Debug") # in Release mode
    set(BULLET_LIBS 
        Bullet2FileLoader
        Bullet3Collision
        Bullet3Common
        Bullet3Dynamics
        Bullet3Geometry
        Bullet3OpenCL_clew
        BulletCollision
        BulletDynamics
        BulletInverseDynamics
        BulletSoftBody
        LinearMath
    )
endif (CMAKE_BUILD_TYPE STREQUAL "Debug")

# set include dirs
include_directories(${BULLET_INCLUDE_DIRS})

# link libraries
link_directories(${BULLET_LIBRARY_DIRS})
link_libraries(${BULLET_LIBS})

# install .dll/.so
set(BULLET_BINARY_DIRS ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})

if (UNIX)
	set(BIN_FILE_PATTERN "*.so*")
else (UNIX)
	set(BIN_FILE_PATTERN "*.dll*")
endif (UNIX)

install(DIRECTORY ${BULLET_BINARY_DIRS}/ DESTINATION ${BULLET_BINARY_DIRS} USE_SOURCE_PERMISSIONS FILES_MATCHING PATTERN ${BIN_FILE_PATTERN})