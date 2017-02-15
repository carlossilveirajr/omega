cmake_minimum_required(VERSION 3.3)
project(omega)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(SOURCE_FILES
    src/Discret.cpp
    src/Discret.h
    src/main.cpp
    src/Reader.cpp
    src/Reader.h
    src/Util.cpp
    src/Util.h)

add_executable(omega ${SOURCE_FILES})