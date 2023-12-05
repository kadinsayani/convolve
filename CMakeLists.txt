cmake_minimum_required(VERSION 3.5)
project(convolve)

set(CMAKE_CXX_FLAGS "-O3 -std=c++11")

add_subdirectory(src)
add_subdirectory(tests)