# Set the CMake version.
cmake_minimum_required (VERSION 2.6)

# Set the project name.
set ( APP_NAME "circle-flow" )
project ( ${APP_NAME} )

# Set the compiler flags.
set ( CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-O3" )

# Set the INCLUDE path.
include_directories ( ${CMAKE_CURRENT_SOURCE_DIR}/include/libharu )

# Include the extra libraries.
set ( EXTRA_LIBS libhpdfs.a z)
link_directories( ${CMAKE_CURRENT_SOURCE_DIR}/lib )

# Add source code.
file ( GLOB SOURCES ./src/*.cpp )

add_executable ( ${APP_NAME} ${SOURCES} )
target_link_libraries ( ${APP_NAME} ${EXTRA_LIBS} )
