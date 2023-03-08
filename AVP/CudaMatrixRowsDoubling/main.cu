#include <iostream>
#include <chrono>
#include <iomanip>
#include <curand.h>

using namespace std;
using namespace std::chrono;

using DataType = float;

constexpr size_t M = 7485;
constexpr size_t N = 9889;

DataType h_inputMatrix[N][M];
DataType h_outputMatrix[N][2 * M];

DataType cpu_outputMatrix[N][2 * M];

void AssertCudaSuccess(cudaError status, std::string_view functionName) {
    if (status != cudaError_t::cudaSuccess) {
        throw std::runtime_error(std::string{functionName} + " error");
    }
}

void AssertCurandSuccess(curandStatus status, std::string_view functionName) {
    if (status != curandStatus::CURAND_STATUS_SUCCESS) {
        throw std::runtime_error(std::string{functionName} + " error");
    }
}

void CudaFillRandom(DataType *matrix) {
    curandGenerator_t generator;

    AssertCurandSuccess(curandCreateGenerator(
                                &generator, CURAND_RNG_PSEUDO_DEFAULT),
                        "curandCreateGenerator");
    AssertCurandSuccess(curandSetPseudoRandomGeneratorSeed(generator, 1234ULL),
                        "curandSetPseudoRandomGeneratorSeed");
    AssertCurandSuccess(curandGenerateUniform(generator, matrix, N * M),
                        "curandGenerateUniform");
}

void PrintDeviceProperties() {
    int device;
    AssertCudaSuccess(cudaGetDevice(&device),
                      "cudaGetDevice");

    cudaDeviceProp props{};
    AssertCudaSuccess(cudaGetDeviceProperties(&props, device),
                      "cudaGetDeviceProperties");

    std::cout << "totalGlobalMem : " << props.totalGlobalMem / 1024 / 1024 << " mb\n";
}

__global__ void KernelDoubleRows(const DataType *inputMatrix,
                                 DataType *outputMatrix,
                                 int rows,
                                 int columns) {
    auto row = blockIdx.x * blockDim.x + threadIdx.x;

    if (row < rows) {
        for (int col = 0; col < columns; col++) {
            auto baseRow = row * columns;

            outputMatrix[2 * (baseRow + col)] = inputMatrix[baseRow + col];
            outputMatrix[2 * (baseRow + col) + 1] = inputMatrix[baseRow + col];
        }
    }
}


void CpuDoubleRows(const DataType input_matrix[N][M],
                   DataType output_matrix[N][2 * M]) {
    auto start = high_resolution_clock::now();
    for (int row = 0; row < N; row++) {
        for (int col = 0; col < M; col++) {
            output_matrix[row][2 * col] = input_matrix[row][col];
            output_matrix[row][2 * col + 1] = input_matrix[row][col];
        }
    }
    std::cout << "CPU time: " << std::setprecision(6) <<
              duration_cast<milliseconds>(high_resolution_clock::now() - start).count()
              << " ms\n";
}

void GpuDoubleRows(DataType *inputMatrix, DataType *outputMatrix) {
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    cudaEventRecord(start);

    dim3 threadsPerBlock(512);
    dim3 numBlocks((N + threadsPerBlock.x - 1) / threadsPerBlock.x);

    KernelDoubleRows<<<numBlocks, threadsPerBlock>>>(
            inputMatrix, outputMatrix, N, M);

    cudaEventSynchronize(stop);
    float milliseconds;
    cudaEventElapsedTime(&milliseconds, start, stop);

    std::cout << "GPU time: " << std::setprecision(6) << milliseconds << " ms\n";
}

bool CompareResults(const DataType cpuMatrix[N][2 * M],
                    const DataType gpuMatrix[N][2 * M]) {
    for (int i = 0; i < N; i++) {
        if (!std::equal(&cpuMatrix[i][0],
                        &cpuMatrix[i][2 * M - 1],
                        &gpuMatrix[i][0])) {
            return false;
        }
    }
    return true;
}


int main() {
    PrintDeviceProperties();

    DataType *d_inputMatrix, *d_outputMatrix;

    AssertCudaSuccess(cudaMalloc((void **) &d_inputMatrix,
                                 N * M * sizeof(DataType)),
                      "cudaMalloc [dev input]");

    CudaFillRandom(d_inputMatrix);

    AssertCudaSuccess(cudaMemcpy(h_inputMatrix, d_inputMatrix,
                                 N * M * sizeof(int), cudaMemcpyDeviceToHost),
                      "cudaMemcpy [dev input]");

    CpuDoubleRows(h_inputMatrix, cpu_outputMatrix);

    AssertCudaSuccess(cudaMalloc((void **) &d_outputMatrix,
                                 N * 2 * M * sizeof(DataType)),
                      "cudaMalloc [dev output]");

    AssertCudaSuccess(cudaMemcpy(d_inputMatrix, h_inputMatrix,
                                 N * M * sizeof(int), cudaMemcpyHostToDevice),
                      "cudaMemcpy [dev input]");

    GpuDoubleRows(d_inputMatrix, d_outputMatrix);

    AssertCudaSuccess(cudaMemcpy(h_outputMatrix,
                                 d_outputMatrix,
                                 N * 2 * M * sizeof(int),
                                 cudaMemcpyDeviceToHost),
                      "cudaMemcpy [host output]");

    std::cout << "\nResult matrices equal: " << std::boolalpha <<
              CompareResults(cpu_outputMatrix,
                             h_outputMatrix) << "\n";

    AssertCudaSuccess(cudaFree(d_inputMatrix),
                      "cudaFree [dev input]");

    AssertCudaSuccess(cudaFree(d_outputMatrix),
                      "cudaFree [dev output]");

    return 0;
}
