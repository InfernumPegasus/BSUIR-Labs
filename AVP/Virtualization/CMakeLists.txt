cmake_minimum_required(VERSION 3.24)
project(Virtualization)

set(CMAKE_CXX_STANDARD 23)

add_compile_options(-mavx2)
add_executable(Virtualization main.cpp)
