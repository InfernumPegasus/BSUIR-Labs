import math

import matplotlib.pyplot as plt
import numpy as np
from PIL import Image, ImageFilter
from skimage import io, measure


def mark_objects(image: Image.Image):
    new_image = image.copy()
    current_label = 0
    free_labels = []

    pixels = new_image.load()

    for x in range(new_image.size[0]):
        for y in range(new_image.size[1]):
            kn = y - 1
            b = pixels[x, kn] if kn > 0 else 0

            km = x - 1
            c = pixels[km, y] if km > 0 else 0

            a = pixels[x, y]

            if a != 0:
                if b == 0 and c == 0:
                    if len(free_labels) > 0:
                        label = free_labels.pop()
                    else:
                        current_label += 1
                        label = current_label

                    pixels[x, y] = label
                elif b != 0 and c == 0:
                    pixels[x, y] = b
                elif b == 0 and c != 0:
                    pixels[x, y] = c
                elif b != 0 and c != 0:
                    pixels[x, y] = b
                    if b != c:
                        change_label(new_image, c, b)
                        free_labels.append(c)

    return new_image


def change_label(image, old_label: int, new_label: int):
    pixels = image.load()

    for x in range(image.size[0]):
        for y in range(image.size[1]):
            if pixels[x, y] == old_label:
                pixels[x, y] = new_label


def binarize(image, threshold):
    pixels = image.load()

    for i in range(image.size[0]):
        for j in range(image.size[1]):
            pixels[i, j] = 0 if pixels[i, j] <= threshold else 255


def generate_unique_labels(image, transform_colors=False):
    matrix = np.array(image.getdata()).reshape(image.size)

    unique_labels = np.unique(matrix)
    unique_labels = np.delete(unique_labels, 0)

    if len(unique_labels) < 25 and transform_colors:
        for i in range(len(unique_labels)):
            change_label(image, int(unique_labels[i]), int(unique_labels[i] * 10))
            unique_labels[i] *= 10

    return unique_labels


def get_region_vectors_skimage(image_name):
    sk_image = io.imread(image_name)
    label_image = measure.label(sk_image)
    regions = measure.regionprops(label_image)

    fig, ax = plt.subplots()
    ax.imshow(sk_image)

    for i, props in enumerate(regions, start=1):
        y0, x0 = props.centroid
        orientation = props.orientation
        x1 = x0 + math.cos(orientation) * 0.5 * props.major_axis_length
        y1 = y0 - math.sin(orientation) * 0.5 * props.major_axis_length
        x2 = x0 - math.sin(orientation) * 0.5 * props.minor_axis_length
        y2 = y0 - math.cos(orientation) * 0.5 * props.minor_axis_length

        ax.plot((x0, x1), (y0, y1), '-r', linewidth=2.5)
        ax.plot((x0, x2), (y0, y2), '-r', linewidth=2.5)
        ax.plot(x0, y0, '.g', markersize=15)

        ax.annotate(
            f'Object {i}',
            xy=(x0, y0),
            arrowprops=dict(facecolor='black', shrink=0.05),
        )

        min_r, min_c, max_r, max_c = props.bbox
        bx = (min_c, max_c, max_c, min_c, min_c)
        by = (min_r, min_r, max_r, max_r, min_r)
        ax.plot(bx, by, '-b', linewidth=2.5)

    plt.plot()
    plt.show()


def main():
    initial_image_name = "image.jpg"
    final_image_name = 'final.pgm'

    initial_image = Image.open(initial_image_name)
    image = initial_image.copy().convert("L")

    plt.imshow(image)
    plt.show()

    binaryish_threshold = 200

    binarize(image, binaryish_threshold)

    plt.imshow(image)
    plt.show()

    new_image = image.filter(ImageFilter.MedianFilter(size=5))
    binarize(new_image, binaryish_threshold)

    plt.imshow(new_image)
    plt.show()

    change_label(new_image, 255, 1)
    marked_image = mark_objects(new_image)
    unique_labels = generate_unique_labels(marked_image, transform_colors=True)

    print('Unique labels:', unique_labels)

    marked_image.save(final_image_name)

    plt.imshow(marked_image)
    plt.show()

    get_region_vectors_skimage(final_image_name)


if __name__ == '__main__':
    main()
