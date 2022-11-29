cmake_minimum_required(VERSION 3.23)
project(USBDevicesUtility)

set(CMAKE_CXX_STANDARD 17)

add_executable(USBDevicesUtility main.cpp)

target_link_libraries(${PROJECT_NAME} udev)