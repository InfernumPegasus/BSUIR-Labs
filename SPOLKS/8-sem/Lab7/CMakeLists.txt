CMAKE_MINIMUM_REQUIRED(VERSION 3.26)
PROJECT(Lab7 CXX)

set(CMAKE_CXX_STANDARD 23)

find_package(MPI REQUIRED)
include_directories(${MPI_CXX_INCLUDE_DIRS})

add_executable(Lab7 main.cpp)

target_link_libraries(Lab7 ${MPI_CXX_LIBRARIES})
