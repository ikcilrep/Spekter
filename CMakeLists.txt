cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_C_STANDARD 17)

#set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

project(spekter CXX)
enable_testing()

set(headers include/spekter.h)

set(sources src/main.cpp)

add_executable(spekter ${sources} ${headers})

add_subdirectory(test)