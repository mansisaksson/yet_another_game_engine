message(STATUS "Configuring GLEW")

include(ExternalProject)

find_package(OpenGL REQUIRED)

set(GLEW_PREFIX glew)

ExternalProject_Add ( glew_download
    PREFIX ${GLEW_PREFIX}
    SOURCE_DIR "${GLEW_PREFIX}/src/glew" # Explicitely set download dir
    URL ${CMAKE_CURRENT_SOURCE_DIR}/third_party/glew.zip
    CONFIGURE_COMMAND "" # Disable all other steps
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)

ExternalProject_Add ( glew
    PREFIX ${GLEW_PREFIX}
    DOWNLOAD_COMMAND "" # Disable download step
    SOURCE_DIR "${GLEW_PREFIX}/src/glew/build/cmake" # Source dir for configuration
    LOG_DOWNLOAD 1
    LOG_BUILD 1
    DEPENDS glew_download
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
        -DBUILD_UTILS:BOOL=ON
        -DGLEW_REGAL:BOOL=FALSE
        -DGLEW_OSMESA:BOOL=FALSE
)

# get the unpacked source/binary directory path
ExternalProject_Get_Property(glew INSTALL_DIR)

# set the include/lib/bin directory variables
set(GLEW_INCLUDE_DIRS ${INSTALL_DIR}/include)
set(GLEW_LIBRARY_DIRS ${INSTALL_DIR}/lib)
set(GLEW_BINARY_DIRS ${INSTALL_DIR}/bin)

# link the correct GLEW directory when the project is in Debug or Release mode
if (CMAKE_BUILD_TYPE STREQUAL "Debug") # in Debug mode
    set(GLEW_LIBS 
        glew32d 
        libglew32d 
        opengl32
    )
else (CMAKE_BUILD_TYPE STREQUAL "Debug") # in Release mode
    set(GLEW_LIBS
        glew32
        libglew32
        opengl32
    )
endif (CMAKE_BUILD_TYPE STREQUAL "Debug")

# set include dirs
include_directories(${GLEW_INCLUDE_DIRS})

# link libraries
link_directories(${GLEW_LIBRARY_DIRS})
link_libraries(${GLEW_LIBS})

# install .dll/.so
set(GLEW_INSTALL_DIR ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})

if (UNIX)
	set(BIN_FILE_PATTERN "*.so*")
else (UNIX)
	set(BIN_FILE_PATTERN "*.dll*")
endif (UNIX)

install(DIRECTORY ${GLEW_BINARY_DIRS}/ DESTINATION ${GLEW_INSTALL_DIR} USE_SOURCE_PERMISSIONS FILES_MATCHING PATTERN ${BIN_FILE_PATTERN})