cmake_minimum_required(VERSION 3.23)
project(PCIDevices C)

set(CMAKE_C_STANDARD 17)
add_link_options(-lpci)

add_executable(PCIDevices main.c)
