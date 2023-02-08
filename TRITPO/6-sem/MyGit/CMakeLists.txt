cmake_minimum_required(VERSION 3.24)
project(MyGit)

set(CMAKE_CXX_STANDARD 23)

find_package(nlohmann_json 3.2.0 REQUIRED)

add_executable(MyGit
        main.cpp
        entity/Commit.h
        entity/file/File.h
        entity/repository/Repository.h
        entity/file/FileStatus.h)

target_link_libraries(nlohmann_json::nlohmann_json)