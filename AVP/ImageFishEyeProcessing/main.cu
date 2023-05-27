#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"
#include <cmath>
#include <stdexcept>

constexpr size_t MARKER = 0xFF00FF;
constexpr dim3 BLOCK_DIM{16, 16};

#define CUDA_ASSERT(cudaError)                                  \
  if (cudaError != cudaSuccess) {                               \
    throw std::runtime_error{cudaGetErrorString(cudaError)};    \
  }

struct Pixel {
    uint8_t r, g, b, a;

    __host__
    __device__
    Pixel() = default;

    __device__
    Pixel(const uint8_t *image,
          const int x,
          const int y,
          const size_t width,
          const int channels) {
        r = image[x * width + y * channels];
        g = image[x * width + y * channels + 1];
        b = image[x * width + y * channels + 2];
        a = image[x * width + y * channels + 3];
    }

    __device__
    Pixel(uint8_t r,
          uint8_t g,
          uint8_t b,
          uint8_t a) : r(r), g(g), b(b), a(a) {}
};

__device__
int2 operator-(int2 a, int2 b) {
    return {a.x - b.x, a.y - b.y};
}

__device__
float2 operator-(float2 a, float2 b) {
    return {a.x - b.x, a.y - b.y};
}

__device__
float operator/(float2 a, float2 b) {
    return a.x / b.x +
           a.y / b.y;
}

__device__
Pixel operator*(Pixel a, float b) {
    a.r = static_cast<uint8_t>(static_cast<float>(a.r) * b);
    a.g = static_cast<uint8_t>(static_cast<float>(a.g) * b);
    a.b = static_cast<uint8_t>(static_cast<float>(a.b) * b);
    a.a = static_cast<uint8_t>(static_cast<float>(a.a) * b);
    return a;
}

__device__
Pixel operator+(Pixel a, Pixel b) {
    a.r = a.r + b.r;
    a.g = a.g + b.g;
    a.b = a.b + b.b;
    a.a = a.a + b.a;
    return a;
}

__device__
bool pixel_check(const uint8_t *image,
                 size_t posX) {
    return (image[posX] << 16) +
           (image[posX + 1] << 8) +
           image[posX + 2] == MARKER;
}

__host__
__device__
float normalize_value(const float value,
                      const float max) {
    return (value - ((max + 1.0f) / 2.0f)) * 2.0f / (max);
}

float calc_coefficient(const float width,
                       const float height,
                       const float radius) {
    const float normalized_source_circle_pointX = normalize_value(height / 2.0f, height);
    const float normalized_source_circle_pointY = normalize_value(width / 2.0f + radius, width);
    const float normalized_source_circle_radius = sqrt(
            normalized_source_circle_pointX * normalized_source_circle_pointX +
            normalized_source_circle_pointY * normalized_source_circle_pointY);

    const float normalized_target_circle_pointY =
            normalize_value(width / 2.0f, width);
    const float normalized_target_circle_pointX =
            normalize_value(height / 2.0f + std::min(width, height) * 0.1f, height);
    const float normalized_target_circle_radius = sqrt(
            normalized_target_circle_pointX * normalized_target_circle_pointX +
            normalized_target_circle_pointY * normalized_target_circle_pointY);

    return log(normalized_target_circle_radius) / log(normalized_source_circle_radius);
}

__device__
float aggregate_coefficient(uint4 mask) {
    if ((mask.x + mask.y + mask.z + mask.w) == 0) {
        return 0;
    } else {
        return 1.0f / static_cast<float>(mask.x + mask.y + mask.z + mask.w);
    }
}

// bilenear interpolation for float point
__device__
void interpolate(uint8_t *input,
                 const float _x,
                 const float _y,
                 const int x,
                 const int y,
                 const int width,
                 const int height,
                 const int channels,
                 const size_t pitch,
                 Pixel &pixel) {
    const int x0 = x;
    const int y0 = y;
    const int x1 = x + 1 < height ? x + 1 : x;
    const int y1 = y + 1 < width ? y + 1 : y;

    const float local_x = _x - trunc(_x);
    const float local_y = _y - trunc(_y);

    Pixel c00(input, x0, y0, pitch, channels);
    Pixel c10(input, x1, y0, pitch, channels);
    Pixel c01(input, x0, y1, pitch, channels);
    Pixel c11(input, x1, y1, pitch, channels);

    pixel = {c00 * ((1 - local_x) * (1 - local_y)) +
             c10 * (local_x * (1 - local_y)) +
             c01 * (local_y * (1 - local_x)) +
             c11 * (local_x * local_y)};
    pixel.a = 255;
}

__device__
void remove_arcs(uint8_t *image,
                 const int x,
                 const int y,
                 const int width,
                 const int height,
                 const int channels,
                 Pixel *pixel) {
    const int x0 = x - 1 > 0 ? x - 1 : x;
    const int y0 = y - 1 > 0 ? y - 1 : y;
    const int x1 = x + 1 < height ? x + 1 : x;
    const int y1 = y + 1 < width ? y + 1 : y;

    uint4 mask{1, 1, 1, 1};

    Pixel c00{image[(x0 * width + y0) * channels],
              image[(x0 * width + y0) * channels + 1],
              image[(x0 * width + y0) * channels + 2],
              image[(x0 * width + y0) * channels + 3]};

    Pixel c10{image[(x1 * width + y0) * channels],
              image[(x1 * width + y0) * channels + 1],
              image[(x1 * width + y0) * channels + 2],
              image[(x1 * width + y0) * channels + 3]};

    Pixel c01{image[(x0 * width + y1) * channels],
              image[(x0 * width + y1) * channels + 1],
              image[(x0 * width + y1) * channels + 2],
              image[(x0 * width + y1) * channels + 3]};

    Pixel c11{image[(x1 * width + y1) * channels],
              image[(x1 * width + y1) * channels + 1],
              image[(x1 * width + y1) * channels + 2],
              image[(x1 * width + y1) * channels + 3]};

    if ((c00.a + c00.b + c00.g + c00.r) == 0) {
        mask.x = 0;
    }
    if ((c10.a + c10.b + c10.g + c10.r) == 0) {
        mask.y = 0;
    }
    if ((c01.a + c01.b + c01.g + c01.r) == 0) {
        mask.z = 0;
    }
    if ((c11.a + c11.b + c11.g + c11.r) == 0) {
        mask.w = 0;
    }

    const float coefficient = aggregate_coefficient(mask);

    *pixel = {c00 * static_cast<float>(mask.x) * coefficient +
              c10 * static_cast<float>(mask.y) * coefficient +
              c01 * static_cast<float>(mask.z) * coefficient +
              c11 * static_cast<float>(mask.w) * coefficient};
    pixel->a = 255;
}

__global__
void recover(uint8_t *image,
             int width,
             int height,
             int channels,
             const bool *mask) {
    const unsigned int x = threadIdx.x + blockDim.x * blockIdx.x;
    const unsigned int y = threadIdx.y + blockDim.y * blockIdx.y;

    if (y > width && x > height) {
        return;
    }
    if (!mask[x * width + y]) {
        Pixel p{};
        remove_arcs(image,
                    static_cast<int>(x), static_cast<int>(y),
                    width, height,
                    channels,
                    &p);

        const auto pos = (x * width + y) * channels;
        image[pos] = p.r;
        image[pos + 1] = p.g;
        image[pos + 2] = p.b;
        image[pos + 3] = p.a;
    }
}

unsigned int collect_accumulator(const unsigned int *accum,
                                 const int min_side) {
    unsigned int max_R = accum[0];
    for (int i = 1; i < min_side; i++) {
        if (max_R < accum[i]) {
            max_R = i;
        }
    }
    return max_R;
}

__global__
void hough_transform(unsigned char *src,
                     size_t width,
                     size_t height,
                     size_t channels,
                     unsigned int *accumulator) {
    const auto x = (threadIdx.x + blockDim.x * blockIdx.x);
    const auto y = threadIdx.y + blockDim.y * blockIdx.y;

    const auto x0 = height / 2;
    const auto y0 = width / 2;
    unsigned int R;

    if (!pixel_check(src, (x * width + y) * channels)) {
        return;
    }
    R = static_cast<int>(ceil(sqrt(static_cast<float>(((x - x0) * (x - x0)) + ((y - y0) * (y - y0))))));
    atomicAdd(accumulator + R, 1U);
}

__global__
void fish_eye(uint8_t *image,
              uint8_t *output,
              const int width,
              const int height,
              int channels,
              const float coefficient,
              const size_t im_pitch,
              bool *mask) {
    const auto x = threadIdx.x + blockDim.x * blockIdx.x;
    const auto y = threadIdx.y + blockDim.y * blockIdx.y;
    if (y > width && x > height) {
        return;
    }

    constexpr float e = 0.001f;
    const float x0 = normalize_value(static_cast<float>(x), static_cast<float>(height));
    const float y0 = normalize_value(static_cast<float>(y), static_cast<float>(width));
    const float r = sqrt(x0 * x0 + y0 * y0);
    const float theta = atan2(y0, x0);
    const float scale = min(1.0f / abs(cos(theta) + e), 1.0f / abs(sin(theta) + e));
    const float new_r = min(scale, 1.0f) * pow(r, coefficient);

    const float new_x = static_cast<float>(height) / 2.0f * new_r * cos(theta) +
                  (static_cast<float>(height) + 1.0f) / 2.0f;
    const float new_y = static_cast<float>(width) / 2.0f * new_r * sin(theta) +
                  (static_cast<float>(width) + 1.0f) / 2.0f;

    if (static_cast<unsigned int>(new_x) < height && new_x >= 0 && static_cast<unsigned int>(new_y) < width &&
        new_y >= 0) {
        Pixel p{};
        interpolate(image,
                    new_x, new_y,
                    static_cast<int>(x), static_cast<int>(y),
                    width, height,
                    channels,
                    im_pitch,
                    p);
        const int xRounded = static_cast<int>(round(new_x));
        const int yRounded = static_cast<int>(round(new_y));
        const int pos = (xRounded * width + yRounded) * channels;

        output[pos] = p.r;
        output[pos + 1] = p.g;
        output[pos + 2] = p.b;
        output[pos + 3] = p.a;
        mask[xRounded * width + yRounded] = true;
    }
}

int main() {
    int width, height, numComp;
    unsigned char *image = stbi_load("../files/zebra3.png", &width, &height, &numComp, 0);
    unsigned char *input_dev_image, *output_dev_image;

    size_t in_pitch, out_pitch;
    CUDA_ASSERT(cudaMallocPitch(&input_dev_image, &in_pitch, width * numComp * sizeof(unsigned char), height))
    CUDA_ASSERT(cudaMallocPitch(&output_dev_image, &out_pitch, width * numComp * sizeof(unsigned char), height))
    CUDA_ASSERT(cudaMemcpy2D(input_dev_image, in_pitch, image, width * numComp * sizeof(unsigned char),
                             width * numComp * sizeof(unsigned char), height, cudaMemcpyHostToDevice))

    const dim3 GRID_DIM{(height + BLOCK_DIM.x - 1) / BLOCK_DIM.x,
                        (width + BLOCK_DIM.y - 1) / BLOCK_DIM.y};

    const auto min = std::min(width, height);
    auto *accum = new unsigned int[min];
    memset(accum, 0, sizeof(unsigned int) * min);
    unsigned int *dev_accum;
    CUDA_ASSERT(cudaMalloc(&dev_accum, min * sizeof(unsigned int)))
    CUDA_ASSERT(cudaMemcpy(dev_accum, accum, min * sizeof(unsigned int), cudaMemcpyHostToDevice))

    hough_transform<<<GRID_DIM, BLOCK_DIM>>>(input_dev_image, width, height, numComp, dev_accum);
    CUDA_ASSERT(cudaMemcpy(accum, dev_accum, min * sizeof(unsigned int), cudaMemcpyDeviceToHost))
    const auto radius = collect_accumulator(accum, min);
    const float coefficient = calc_coefficient(static_cast<float>(width),
                                               static_cast<float>(height),
                                               static_cast<float>(radius));

    bool *host_mask = new bool[width * height];
    bool *dev_mask;
    memset(host_mask, false, sizeof(bool) * width * height);
    CUDA_ASSERT(cudaMalloc(&dev_mask, width * height * sizeof(bool)))
    CUDA_ASSERT(cudaMemcpy(dev_mask, host_mask, width * height * sizeof(bool), cudaMemcpyHostToDevice))
    fish_eye<<<GRID_DIM, BLOCK_DIM, sizeof(Pixel) * BLOCK_DIM.x * BLOCK_DIM.y>>>(
            input_dev_image,
            output_dev_image,
            width, height,
            numComp,
            coefficient,
            in_pitch,
            dev_mask);

    recover<<<GRID_DIM, BLOCK_DIM, sizeof(Pixel) * (BLOCK_DIM.x + 2) * (BLOCK_DIM.y + 2)>>>(
            output_dev_image,
            width, height, numComp, dev_mask);
    CUDA_ASSERT(cudaMemcpy(host_mask, dev_mask, width * height * sizeof(bool), cudaMemcpyDeviceToHost))

    auto *host_image = new unsigned char[width * numComp * sizeof(unsigned char) * height];
    CUDA_ASSERT(cudaMemcpy2D(host_image, width * sizeof(unsigned char) * numComp, output_dev_image,
                             width * sizeof(unsigned char) * numComp, width * sizeof(unsigned char) * numComp, height,
                             cudaMemcpyDeviceToHost))
    stbi_write_png("../files/result.png", width, height, 4, host_image, width * 4);
    stbi_image_free(image);

    delete[] accum;
    delete[] host_mask;

    CUDA_ASSERT(cudaFree(dev_mask))
    CUDA_ASSERT(cudaFree(dev_accum))
    CUDA_ASSERT(cudaFree(input_dev_image))
    CUDA_ASSERT(cudaFree(output_dev_image))

    return 0;
}