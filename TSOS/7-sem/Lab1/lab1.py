import sys

import cv2
import matplotlib.pyplot as plt
import numpy as np


def plot_histogram_1(image1):
    plt.figure()
    plt.hist(image1.ravel(), 256, [0, 256])
    plt.show()


def plot_histogram_2(image1, image2):
    plt.figure()

    plt.subplot(2, 1, 1)
    plt.hist(image1.ravel(), 256, [0, 256])

    plt.subplot(2, 1, 2)
    plt.hist(image2.ravel(), 256, [0, 256])

    plt.show()


def apply_prewitt_filter(image):
    gray_image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

    dx = cv2.Sobel(gray_image, cv2.CV_64F, 1, 0, ksize=3)
    dy = cv2.Sobel(gray_image, cv2.CV_64F, 0, 1, ksize=3)

    gradient_x = cv2.convertScaleAbs(dx)
    gradient_y = cv2.convertScaleAbs(dy)

    gradient = cv2.addWeighted(gradient_x, 0.5, gradient_y, 0.5, 0)

    return gradient


def apply_prewitt_filter_no_lib(image):
    height, width, _ = image.shape

    kernel_x = np.array([[-1, 0, 1], [-1, 0, 1], [-1, 0, 1]])
    kernel_y = np.array([[-1, -1, -1], [0, 0, 0], [1, 1, 1]])

    gray_image = np.zeros((height, width), dtype=np.uint8)
    for i in range(height):
        for j in range(width):
            gray_image[i, j] = 0.2989 * image[i, j, 2] + 0.5870 * image[i, j, 1] + 0.1140 * image[i, j, 0]

    filtered_image = np.copy(gray_image)

    for i in range(1, height - 1):
        for j in range(1, width - 1):
            gx = np.sum(np.multiply(kernel_x, gray_image[i - 1:i + 2, j - 1:j + 2]))
            gy = np.sum(np.multiply(kernel_y, gray_image[i - 1:i + 2, j - 1:j + 2]))
            filtered_image[i, j] = np.sqrt(gx ** 2 + gy ** 2)

    return filtered_image


def change_contrast_benchmark(image, min_brightness, max_brightness, contrast_factor):
    res = cv2.convertScaleAbs(image, alpha=contrast_factor, beta=max_brightness - min_brightness)
    return res


def increase_contrast(image, min_brightness, max_brightness, contrast_factor):
    contrast_image = np.copy(image)
    for i in range(image.shape[0]):
        for j in range(image.shape[1]):
            for k in range(image.shape[2]):
                contrast_image[i, j, k] = np.clip(contrast_factor * image[i, j, k] + (max_brightness - min_brightness),
                                                  0, 255)

    return contrast_image


def main(min_brightness, max_brightness, contrast_factor):
    image = cv2.imread('bettercat.jpg')

    contrast_image_bench = change_contrast_benchmark(image, min_brightness, max_brightness, contrast_factor)
    increase_image = increase_contrast(image, min_brightness, max_brightness, contrast_factor)

    prewitt_image1 = apply_prewitt_filter(image)
    prewitt_image2 = apply_prewitt_filter_no_lib(image)

    cv2.imshow('Original Image', image)
    cv2.imshow('Bench Image', contrast_image_bench)
    cv2.imshow('Increase Image', increase_image)
    cv2.waitKey(0)
    cv2.destroyAllWindows()
    cv2.imshow('Prewitt Image', prewitt_image1)
    cv2.imshow('Prewitt Image no lib', prewitt_image2)
    cv2.waitKey(0)
    cv2.destroyAllWindows()
    plot_histogram_2(image, increase_image)
    plot_histogram_1(prewitt_image2)
    cv2.waitKey(0)


if __name__ == "__main__":
    if len(sys.argv) != 4:
        print('Using default values!')
        min_value = 50
        max_value = 150
        contrast = 1.2
    else:
        min_value = int(sys.argv[1])
        max_value = int(sys.argv[2])
        contrast = float(sys.argv[3])
    main(min_value, max_value, contrast)

