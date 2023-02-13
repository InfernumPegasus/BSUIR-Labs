cmake_minimum_required(VERSION 3.24)
project(MyGit)

set(CMAKE_CXX_STANDARD 23)

add_executable(MyGit
        main.cpp
        entity/file/File.h entity/file/File.cpp
        entity/commit/Commit.h entity/commit/Commit.cpp
        entity/repository/Repository.h entity/repository/Repository.cpp)