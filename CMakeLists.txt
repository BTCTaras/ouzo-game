CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(ouzo)

# Finds all source files and adds them.
FILE(GLOB OUZO_SRC "src/*.cpp" "include/*.h")
ADD_EXECUTABLE(ouzo ${OUZO_SRC})

# Fetch modules from ./cmake/Modules
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules")

# Enable C++11
IF(UNIX)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")
ENDIF()

SET(BIN_DIR ${OUZO_SOURCE_DIR}/bin)

# Define OUZO_DEBUG in debug mode.
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DOUZO_DEBUG")
