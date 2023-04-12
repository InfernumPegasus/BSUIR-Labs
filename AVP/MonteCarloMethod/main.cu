#include <iostream>
#include <algorithm>
#include <chrono>
#include <curand_kernel.h>

using namespace std;
using namespace chrono;

/*
 * Var 5
 */

void PrintGPUsInfo() {
    int devices;
    cudaGetDeviceCount(&devices);

    if (devices == 0) {
        std::cout << "You have no GPUs!\n";
        return;
    }

    for (int i = 0; i < devices; i++) {
        cudaDeviceProp cudaDeviceProp{};
        cudaGetDeviceProperties(&cudaDeviceProp, i);

        std::cout << "Device '" << cudaDeviceProp.name << "':\n\n";
        std::cout << "max threads per block: "
                  << cudaDeviceProp.maxThreadsPerBlock << "\n";
        std::cout << "totalGlobalMem: "
                  << cudaDeviceProp.totalGlobalMem / 1024 / 1024
                  << " mb\n";
        std::cout << "sharedMemoryPerBlock: "
                  << cudaDeviceProp.sharedMemPerBlock / 1024
                  << " kb.\n";
        std::cout << "compute capabilities: " << cudaDeviceProp.major << " (major), " <<
                  cudaDeviceProp.minor << " (minor)\n";

        auto maxGridSize = cudaDeviceProp.maxGridSize;
        std::cout << "maxGridSize: "
                  << maxGridSize[0] << " x "
                  << maxGridSize[1] << " x "
                  << maxGridSize[2] << "\n";
        std::cout << "compute units: "
                  << cudaDeviceProp.multiProcessorCount << "\n\n";
    }
}

using Point = float3;

constexpr Point A{0.6, 0.7, -0.1};
constexpr Point B{0.1, -1.3, -0.27};
constexpr Point C{-2.1, 0.4, -0.5};
constexpr Point D{0.3, 0.1, 0.4};

constexpr auto X_SET = {A.x, B.x, C.x, D.x};
constexpr auto Y_SET = {A.y, B.y, C.y, D.y};
constexpr auto Z_SET = {A.z, B.z, C.z, D.z};

constexpr float MIN_X = std::min(X_SET);
constexpr float MAX_X = std::max(X_SET);

constexpr float MIN_Y = std::min(Y_SET);
constexpr float MAX_Y = std::max(Y_SET);

constexpr float MIN_Z = std::min(Z_SET);
constexpr float MAX_Z = std::max(Z_SET);

constexpr float CUBE_VOLUME = abs(
        (MAX_X - MIN_X) *
        (MAX_Y - MIN_Y) *
        (MAX_Z - MIN_Z)
);

constexpr auto ITERATIONS = 100'000'000;
constexpr auto ITERATIONS_PER_THREAD = 1000;
constexpr auto TOTAL_USED_THREADS = ITERATIONS / ITERATIONS_PER_THREAD;

constexpr int MAX_THREADS_PER_BLOCK = 1024;
constexpr int THREADS_IN_WARP = 32;
constexpr int WARPS_PER_BLOCK = MAX_THREADS_PER_BLOCK / THREADS_IN_WARP;


class Plane {
public:
    constexpr Plane(
            Point p1,
            Point p2,
            Point p3) :
            a(GetDeterminant2D({p2.y - p1.y, p2.z - p1.z},
                               {p3.y - p1.y, p3.z - p1.z})),
            b(-GetDeterminant2D({p2.x - p1.x, p2.z - p1.z},
                                {p3.x - p1.x, p3.z - p1.z})),
            c(GetDeterminant2D({p2.x - p1.x, p2.y - p1.y},
                               {p3.x - p1.x, p3.y - p1.y})),
            d(-p1.x * a - p1.y * b - p1.z * c) {
        if (d < 0) {
            a *= -1;
            b *= -1;
            c *= -1;
            d *= -1;
        }
    }

    static constexpr float GetDeterminant2D(float2 r1, float2 r2) {
        return r1.x * r2.y -
               r1.y * r2.x;
    }

    static constexpr float GetDeterminant3D(float3 r1, float3 r2, float3 r3) {
        return r1.x * GetDeterminant2D({r2.y, r2.z}, {r3.y, r3.z}) -
               r2.x * GetDeterminant2D({r1.y, r1.z}, {r3.y, r3.z}) +
               r3.x * GetDeterminant2D({r1.y, r1.z}, {r2.y, r2.z});
    }

    [[nodiscard]]
    __host__
    __device__
    constexpr bool ContainsPoint(Point point) const {
        return a * point.x +
               b * point.y +
               c * point.z +
               d >= 0;
    }

public:
    [[maybe_unused]]
    void Info() const {
        std::cout << "A: " << a << "\n"
                  << "B: " << b << "\n"
                  << "C: " << c << "\n"
                  << "D: " << d << "\n";
        std::cout << "Contains {0, 0, 0}: " << std::boolalpha
                  << ContainsPoint({0, 0, 0})
                  << std::noboolalpha << "\n\n";
    }

private:
    float a, b, c, d;
};

__device__
constexpr Plane ABC(A, B, C);
__device__
constexpr Plane ABD(A, B, D);
__device__
constexpr Plane BCD(B, C, D);
__device__
constexpr Plane CAD(C, A, D);

__host__
__device__
constexpr float RandomNumber(float initial, float min, float max) {
    return initial * (max - min) + min;
}

constexpr float AnalyticComputeVolume() {
    constexpr Point AB{B.x - A.x, B.y - A.y, B.z - A.z};
    constexpr Point AC{C.x - A.x, C.y - A.y, C.z - A.z};
    constexpr Point AD{D.x - A.x, D.y - A.y, D.z - A.z};
    return abs(Plane::GetDeterminant3D(AB, AC, AD)) / 6;
}

float CPUComputeVolume() {
    int hits{};
    for (int i = 0; i < ITERATIONS; i++) {
        const auto x = RandomNumber(((float) rand() / (float) RAND_MAX),
                                    MIN_X, MAX_X);
        const auto y = RandomNumber(((float) rand() / (float) RAND_MAX),
                                    MIN_Y, MAX_Y);
        const auto z = RandomNumber(((float) rand() / (float) RAND_MAX),
                                    MIN_Z, MAX_Z);

        const Point point{x, y, z};
        if (ABC.ContainsPoint(point) &&
            ABD.ContainsPoint(point) &&
            BCD.ContainsPoint(point) &&
            CAD.ContainsPoint(point)) {
            hits++;
        }
    }
    return CUBE_VOLUME * (float) hits / (float) ITERATIONS;
}

__managed__
unsigned long long totalHits = 0;

__device__
int ShuffleDownWarp(int value) {
    constexpr auto MASK = 0xffffffff;
    for (int offset = THREADS_IN_WARP / 2; offset > 0; offset /= 2) {
        value += __shfl_down_sync(MASK, value, offset);
    }
    return value;
}

__device__
int SumBlock(int value) {
    __shared__ int reduceBuffer[WARPS_PER_BLOCK];

    const auto warpLine = threadIdx.x % THREADS_IN_WARP;
    const auto warpId = threadIdx.x / THREADS_IN_WARP;

    value = ShuffleDownWarp(value);
    if (warpLine == 0) {
        reduceBuffer[warpId] = value;
    }
    __syncthreads();

    // Loads values back only in the first warp of the first block
    value = (threadIdx.x < blockDim.x / THREADS_IN_WARP) ? reduceBuffer[warpLine] : 0;
    if (warpId == 0) {
        value = ShuffleDownWarp(value);
    }
    return value;
}

__global__
void KernelCountHits() {
    const auto threadId = blockIdx.x * blockDim.x + threadIdx.x;
    if (threadId >= TOTAL_USED_THREADS) {
        return;
    }

    curandState randomState;
    curand_init(1234, threadId, 0, &randomState);

    int hits = 0;
    for (int i = 0; i < ITERATIONS_PER_THREAD; ++i) {
        const Point point{
                RandomNumber(curand_uniform(&randomState), MIN_X, MAX_X),
                RandomNumber(curand_uniform(&randomState), MIN_Y, MAX_Y),
                RandomNumber(curand_uniform(&randomState), MIN_Z, MAX_Z),
        };
        if (ABC.ContainsPoint(point) &&
            ABD.ContainsPoint(point) &&
            BCD.ContainsPoint(point) &&
            CAD.ContainsPoint(point)) {
            ++hits;
        }
    }

    const int blockHits = SumBlock(hits);
    if (threadIdx.x == 0) {
        atomicAdd(&totalHits, blockHits);
    }
}

float GPUComputeVolume() {
    constexpr int THREADS_PER_BLOCK = 128;
    constexpr int BLOCKS_PER_GRID = (TOTAL_USED_THREADS + THREADS_PER_BLOCK - 1) / THREADS_PER_BLOCK;
    KernelCountHits<<<BLOCKS_PER_GRID, THREADS_PER_BLOCK>>>();
    cudaDeviceSynchronize();
    return CUBE_VOLUME * (float) totalHits / (float) ITERATIONS;
}

bool SameResults(float analyticVolume, float CPUVolume, float GPUVolume) {
    return abs(GPUVolume - analyticVolume) < 0.1 &&
           abs(GPUVolume - CPUVolume) < 0.001;
}

int main() {
    srand(time(nullptr));
    PrintGPUsInfo();

//    ABC.Info();
//    ABD.Info();
//    BCD.Info();
//    CAD.Info();

    auto analyticVolume = AnalyticComputeVolume();
    std::cout << "AnalyticComputeVolume: " << analyticVolume << "\n";


    auto start = high_resolution_clock::now();
    auto cpuVolume = CPUComputeVolume();
    std::cout << "CPUComputeVolume: " << cpuVolume << "\n";
    std::cout << "CPU time: "
              << duration_cast<milliseconds>(high_resolution_clock::now() - start).count()
              << " ms\n";


    cudaEvent_t eventStart, eventStop;
    cudaEventCreate(&eventStart);
    cudaEventCreate(&eventStop);
    cudaEventRecord(eventStart);
    auto gpuVolume = GPUComputeVolume();
    std::cout << "GPUComputeVolume: " << gpuVolume << "\n";
    cudaEventSynchronize(eventStop);
    float milliseconds;
    cudaEventElapsedTime(&milliseconds, eventStart, eventStop);
    std::cout << "GPU time: " << milliseconds << " ms\n";

    std::cout << "Same volumes: " << std::boolalpha
              << SameResults(analyticVolume, cpuVolume, gpuVolume) << " \n";
    cudaEventDestroy(eventStart);
    cudaEventDestroy(eventStop);

    return 0;
}
