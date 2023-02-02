cmake_minimum_required(VERSION 3.24)
project(MyGit)

set(CMAKE_CXX_STANDARD 23)

find_package(nlohmann_json 3.2.0 REQUIRED)

add_executable(MyGit
        main.cpp
        vcs/Commit.h
        vcs/File.h
        vcs/Repository.h
        vcs/VersionControlSystem.h
        vcs/Branch.h
        vcs/FileStatus.h vcs/FileDifference.h)

target_link_libraries(nlohmann_json::nlohmann_json)