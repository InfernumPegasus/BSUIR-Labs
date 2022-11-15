cmake_minimum_required(VERSION 3.1)
project(WebcamUtility)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(WebcamUtility main.cpp)

target_link_libraries(WebcamUtility ${OpenCV_LIBS})