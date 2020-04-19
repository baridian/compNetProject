cmake_minimum_required(VERSION 3.13)
project(compNetProject)

set(CMAKE_CXX_STANDARD 14)

add_executable(compNetProject src/main.cpp src/Server.cpp src/Server.h)