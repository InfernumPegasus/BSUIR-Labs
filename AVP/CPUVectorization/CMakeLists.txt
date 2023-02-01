cmake_minimum_required(VERSION 3.24)
project(CPUVectorization)

set(CMAKE_CXX_STANDARD 23)

add_compile_options(-mavx2)
add_executable(CPUVectorization main.cpp)