cmake_minimum_required(VERSION 2.6)
project(SIMPLEAI)

option(SIMPLEAI_EXAMPLE "Compile example program" OFF)
option(SIMPLEAI_TEST "Compile tests" OFF)
option(SIMPLEAI_LUA "Compile with lua support" ON)

if (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
	set(CMAKE_CXX_STANDARD_LIBRARIES "kernel32.lib user32.lib wsock32.lib ws2_32.lib wininet.lib")
	set(CMAKE_C_STANDARD_LIBRARIES "kernel32.lib user32.lib wsock32.lib ws2_32.lib wininet.lib")
elseif (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Darwin")
elseif (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux")
	set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra")
	set(CMAKE_CXX_FLAGS_RELEASE "-O3 -ftree-vectorize -msse3 -ffast-math -DNDEBUG")
	set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -DDEBUG -Werror")
endif()

add_subdirectory(src/ai)
add_subdirectory(src/example)
add_subdirectory(src/test)

find_program(DOXYGEN "doxygen")
if (DOXYGEN)
	message("doxygen found")
else()
	message("doxygen not found")
endif()

# TODO execute doxygen
