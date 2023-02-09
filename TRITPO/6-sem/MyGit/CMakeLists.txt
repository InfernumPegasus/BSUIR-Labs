cmake_minimum_required(VERSION 3.24)
project(MyGit)

set(CMAKE_CXX_STANDARD 23)

find_package(nlohmann_json 3.2.0 REQUIRED)

add_executable(MyGit
        main.cpp
        entity/commit/Commit.h entity/commit/Commit.cpp
        entity/file/File.h entity/file/File.cpp
        entity/repository/Repository.h entity/repository/Repository.cpp
        entity/file/FileStatus.h)

target_link_libraries(${PROJECT_NAME} nlohmann_json::nlohmann_json)