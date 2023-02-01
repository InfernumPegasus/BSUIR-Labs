cmake_minimum_required(VERSION 3.24)
project(MyGit)

set(CMAKE_CXX_STANDARD 23)

add_executable(MyGit main.cpp vcs/Commit.h vcs/File.h vcs/Repository.h vcs/VersionControlSystem.h)
