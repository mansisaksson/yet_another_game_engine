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
    #CONFIGURE_COMMAND ""
    #BUILD_COMMAND ""
    #BUILD_IN_SOURCE 1
    #INSTALL_COMMAND ""
    LOG_DOWNLOAD 1
    LOG_BUILD 1
    #STEP_TARGETS ${GLFW_PREFIX}_info
)

# get the unpacked source directory path
ExternalProject_Get_Property(${GLFW_PREFIX} SOURCE_DIR)
message(STATUS "Source directory of ${GLFW_PREFIX} ${SOURCE_DIR}")

# Set separate directories for building in Debug or Release mode
set(GLFW_DEBUG_DIR ${SOURCE_DIR}/../${GLFW_PREFIX}-build/src/Debug)
set(GLFW_RELEASE_DIR ${SOURCE_DIR}/../${GLFW_PREFIX}-build/src/Release)
message(STATUS "GLFW Debug directory ${GLFW_DEBUG_DIR}")
message(STATUS "GLFW Release directory ${GLFW_RELEASE_DIR}")

# set the include directory variable and include it
set(GLFW_INCLUDE_DIRS ${SOURCE_DIR}/include)
include_directories(${GLFW_INCLUDE_DIRS})

# link the correct GLFW directory when the project is in Debug or Release mode
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	# in Debug mode
	link_directories(${GLFW_DEBUG_DIR})
	set(GLFW_LIBS glfw3dll)
	set(GLFW_LIBRARY_DIRS ${GLFW_DEBUG_DIR})
else (CMAKE_BUILD_TYPE STREQUAL "Debug")
	# in Release mode
	link_directories(${GLFW_RELEASE_DIR})
	set(GLFW_LIBS glfw3dll)
	set(GLFW_LIBRARY_DIRS ${GLFW_RELEASE_DIR})
endif (CMAKE_BUILD_TYPE STREQUAL "Debug")

# verify that the GLFW header files can be included
set(CMAKE_REQUIRED_INCLUDES_SAVE ${CMAKE_REQUIRED_INCLUDES})
set(CMAKE_REQUIRED_INCLUDES ${CMAKE_REQUIRED_INCLUDES} 	${GLFW_INCLUDE_DIRS})
check_include_file_cxx("GLFW/glfw3.h" HAVE_GLFW)
set(CMAKE_REQUIRED_INCLUDES ${CMAKE_REQUIRED_INCLUDES_SAVE})
 if (NOT HAVE_GLFW)
	message(STATUS "Did not build GLFW correctly as cannot find glfw3.h. Will build it.")
	set(HAVE_GLFW 1)
endif (NOT HAVE_GLFW)