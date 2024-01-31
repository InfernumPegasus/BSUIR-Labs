import sys

import cv2 as cv
import numpy as np

TAU = 1.0
HEIGHT = 700
CUTOFF_FREQ = 0.1
ORDER = 4


def wavelet(x):
    return (1 / (np.pi * x)) * (np.sin(2 * np.pi * x) - np.sin(np.pi * x))


def butterworth_low_pass(x, cutoff_freq, order):
    return 1 / (1 + (x / cutoff_freq) ** (2 * order))


def low_pass_kernel(n):
    result = np.zeros((n // 2, n))
    for i in np.arange(n // 2):
        result[i, 2 * i] = butterworth_low_pass(-TAU / 2.0, CUTOFF_FREQ, ORDER)
        result[i, 2 * i + 1] = butterworth_low_pass(TAU / 2.0, CUTOFF_FREQ, ORDER)
    return 1 / np.sqrt(2) * result


def high_pass_kernel(n):
    result = np.zeros((n // 2, n))
    for i in np.arange(n // 2):
        result[i, 2 * i] = butterworth_low_pass(-TAU / 2.0, CUTOFF_FREQ, ORDER)
        result[i, 2 * i + 1] = -butterworth_low_pass(TAU / 2.0, CUTOFF_FREQ, ORDER)
    return 1 / np.sqrt(2) * result


def wavelet_transform(image):
    if image.shape[0] % 2:
        comps = []
        for k in np.arange(image.shape[2]):
            comp = image[:, :, k]
            comp = np.concatenate((comp, np.array([comp[-1, :]])), axis=0)
            comps.append(comp.reshape((comp.shape[0], comp.shape[1], 1)))
        image = np.concatenate(comps, axis=2)

    if image.shape[1] % 2:
        comps = []
        for k in np.arange(image.shape[2]):
            comp = image[:, :, k]
            comp = np.concatenate((comp, np.array([comp[:, -1]]).T), axis=1)
            comps.append(comp.reshape((comp.shape[0], comp.shape[1], 1)))
        image = np.concatenate(comps, axis=2)

    coeff_a, coeff_h, coeff_v, coeff_d = [], [], [], []

    for k in np.arange(image.shape[2]):
        approx = np.dot(image[:, :, k], low_pass_kernel(image.shape[1]).T)
        detail = np.dot(image[:, :, k], high_pass_kernel(image.shape[1]).T)

        a = np.dot(low_pass_kernel(approx.shape[0]), approx)
        h = np.dot(high_pass_kernel(approx.shape[0]), approx)
        v = np.dot(low_pass_kernel(detail.shape[0]), detail)
        d = np.dot(high_pass_kernel(detail.shape[0]), detail)

        h, v, d = np.abs(h), np.abs(v), np.abs(d)
        a, h, v, d = (
            np.uint8(a / np.max(a) * 255.0),
            np.uint8(h / np.max(h) * 255.0),
            np.uint8(v / np.max(v) * 255.0),
            np.uint8(d / np.max(d) * 255.0),
        )
        coeff_a.append(a.reshape((a.shape[0], a.shape[1], 1)))
        coeff_h.append(h.reshape((h.shape[0], h.shape[1], 1)))
        coeff_v.append(v.reshape((v.shape[0], v.shape[1], 1)))
        coeff_d.append(d.reshape((d.shape[0], d.shape[1], 1)))

    a = np.concatenate(coeff_a, axis=2)
    h = np.concatenate(coeff_h, axis=2)
    v = np.concatenate(coeff_v, axis=2)
    d = np.concatenate(coeff_d, axis=2)

    return a, h, v, d


def main():
    path = './image.png'
    image = cv.imread(path, cv.IMREAD_COLOR)
    size = (int(HEIGHT * image.shape[1] / image.shape[0]), HEIGHT)
    coeff_size = (size[0] // 2, size[1] // 2)
    level = int(input('level: '))

    for l in np.arange(level):
        cv.imshow(f'Wavelet Level {l + 1}', cv.resize(image, size, interpolation=cv.INTER_NEAREST))

        a, h, v, d = wavelet_transform(image)

        combined_image = np.concatenate((a, h, v, d), axis=1)
        cv.imshow(f'Approximation + Details {l + 1}',
                  cv.resize(combined_image, (coeff_size[0] * 4, coeff_size[1]), interpolation=cv.INTER_NEAREST))

        print(f'Scale = {4 ** (l + 1)}')
        cv.waitKey(0)
        cv.destroyAllWindows()
        image = a

    sys.exit(0)


if __name__ == '__main__':
    main()
