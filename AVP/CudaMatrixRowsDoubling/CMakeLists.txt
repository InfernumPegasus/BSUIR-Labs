cmake_minimum_required(VERSION 3.24)
project(CudaMatrixRowsDoubling CUDA)

set(CMAKE_CUDA_STANDARD 20)

add_executable(${PROJECT_NAME} main.cu)

set_target_properties(${PROJECT_NAME} PROPERTIES
        CUDA_SEPARABLE_COMPILATION ON)

target_link_libraries(${PROJECT_NAME} -lcurand)