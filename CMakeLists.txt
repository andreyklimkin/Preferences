cmake_minimum_required(VERSION 3.3)
project(Cars_Preferences)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp classes.cpp classes.h Trivial.cpp Trivial.h baseline.cpp baseline.h)
add_executable(Cars_Preferences ${SOURCE_FILES})