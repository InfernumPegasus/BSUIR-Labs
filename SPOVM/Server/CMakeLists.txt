cmake_minimum_required(VERSION 3.24)
project(TCP_Server)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wall -Wpedantic -Wno-maybe-uninitialized -O3")

add_library(global
functions.c
)

add_executable(
        client
        client.c
)

add_executable(
        server
        server.c
)

target_link_libraries(client PRIVATE global)
target_link_libraries(server PRIVATE global)
