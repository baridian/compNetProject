cmake_minimum_required(VERSION 3.13)
project(compNetProject)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_VERBOSE_MAKEFILE ON)

add_executable(compNetProject src/Server.cpp src/Server.h src/server_main.cpp src/Buffer.h src/wrap_string.cpp src/wrap_string.h)

target_link_libraries(compNetProject ws2_32)
