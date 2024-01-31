cmake_minimum_required(VERSION 3.20)
project(server)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wall -Wpedantic -Wno-maybe-uninitialized -O3")

add_executable(
        client
        Client.cpp
)

add_executable(
        server
        Server.cpp
)

target_link_libraries(client PRIVATE)
target_link_libraries(server PRIVATE fmt)
