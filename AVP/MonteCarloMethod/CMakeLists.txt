cmake_minimum_required(VERSION 3.24)
project(MonteCarloMethod CUDA)

set(CMAKE_CUDA_STANDARD 23)

add_executable(${PROJECT_NAME} main.cu)

set_target_properties(MonteCarloMethod PROPERTIES
        CUDA_SEPARABLE_COMPILATION ON)

target_link_libraries(${PROJECT_NAME} -lcurand)