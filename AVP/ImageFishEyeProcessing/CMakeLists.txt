cmake_minimum_required(VERSION 3.24)
project(ImageFishEyeProcessing CUDA)

set(CMAKE_CUDA_STANDARD 23)

add_executable(${PROJECT_NAME} main.cu)

set_target_properties(${PROJECT_NAME} PROPERTIES
        CUDA_SEPARABLE_COMPILATION ON)
