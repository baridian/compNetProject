cmake_minimum_required(VERSION 3.13)
project(compNetProject)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_VERBOSE_MAKEFILE ON)

add_executable(compNetProject src/main.cpp src/Server.cpp src/Server.h)

target_link_libraries(compNetProject ws2_32
