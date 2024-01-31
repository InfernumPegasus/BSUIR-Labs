import cv2 as cv
import numpy as np
from matplotlib import pyplot as plt

from ideal import GREATER_OR_EQUAL, LESS_OR_EQUAL, NOT_EQUAL, EQUIVALENCE, APPROXIMATELY_EQUAL
from network import Perceptron

EPSILON = 0.0005

CHANCE = 0.1


def noise(image, chance):
    """
    Returns a noisy `image` determined by `chance`.
    """
    result = image.copy()

    for i, x in np.ndenumerate(result):
        if np.random.random() < chance:
            result[i] = 255 - x

    return result


def binarize(image):
    """
    Implements an `image` binarization.
    """
    result = image.copy()

    for i, x in np.ndenumerate(result):
        result[i] = (255 - x) / 255

    return result.reshape((1, result.size))


def main():
    """
    The main function of the program.
    """
    np.set_printoptions(formatter={
        'float': lambda x: f'{x:6.3f}'
    })

    percept = Perceptron(36, 20, 5)

    training_data = [
        (binarize(GREATER_OR_EQUAL).T, np.array([[1, 0, 0, 0, 0]]).T),
        (binarize(LESS_OR_EQUAL).T, np.array([[0, 1, 0, 0, 0]]).T),
        (binarize(NOT_EQUAL).T, np.array([[0, 0, 1, 0, 0]]).T),
        (binarize(EQUIVALENCE).T, np.array([[0, 0, 0, 1, 0]]).T),
        (binarize(APPROXIMATELY_EQUAL).T, np.array([[0, 0, 0, 0, 1]]).T),
    ]

    n = len(training_data)

    # Total number of iterations.
    iterations = 0

    while True:
        np.random.shuffle(training_data)

        percept.gradient_descent(training_data)

        # Error function of the `percept`.
        delta = np.zeros((n, 1))

        for data in training_data:
            delta += (data[1] - percept.feed_forward(data[0])) ** 2

        if np.all(delta / n < EPSILON):
            break

        iterations += 1

    print(f'iterations: {iterations}')

    test_data = [
        noise(GREATER_OR_EQUAL, CHANCE), noise(GREATER_OR_EQUAL, CHANCE), noise(GREATER_OR_EQUAL, CHANCE),
        noise(LESS_OR_EQUAL, CHANCE), noise(LESS_OR_EQUAL, CHANCE), noise(LESS_OR_EQUAL, CHANCE),
        noise(NOT_EQUAL, CHANCE), noise(NOT_EQUAL, CHANCE), noise(NOT_EQUAL, CHANCE),
        noise(EQUIVALENCE, CHANCE), noise(EQUIVALENCE, CHANCE), noise(EQUIVALENCE, CHANCE),
        noise(APPROXIMATELY_EQUAL, CHANCE), noise(APPROXIMATELY_EQUAL, CHANCE), noise(APPROXIMATELY_EQUAL, CHANCE),
    ]

    for i, test in enumerate(test_data):
        print(f'{i + 1:2d}: {100 * percept.feed_forward(binarize(test).T).T}')

    for i, image in enumerate(test_data):
        cv.imshow(f'Test {i + 1}', cv.resize(
            image,
            (256, 256),
            interpolation=cv.INTER_NEAREST
        ))

    show_ideal()
    cv.waitKey(0)
    cv.destroyAllWindows()


def show_ideal():
    image1 = cv.resize(GREATER_OR_EQUAL, (256, 256), interpolation=cv.INTER_NEAREST)
    image2 = cv.resize(LESS_OR_EQUAL, (256, 256), interpolation=cv.INTER_NEAREST)
    image3 = cv.resize(NOT_EQUAL, (256, 256), interpolation=cv.INTER_NEAREST)
    image4 = cv.resize(EQUIVALENCE, (256, 256), interpolation=cv.INTER_NEAREST)
    image5 = cv.resize(APPROXIMATELY_EQUAL, (256, 256), interpolation=cv.INTER_NEAREST)
    images = [image1, image2, image3, image4, image5]

    fig, axes = plt.subplots(1, 5, figsize=(15, 3))
    for i, ax in enumerate(axes):
        if i < len(images):
            image = cv.resize(images[i], (256, 256), interpolation=cv.INTER_NEAREST)
            ax.imshow(image)
            ax.set_title(f'Image {i + 1}')
        else:
            fig.delaxes(ax)
    plt.tight_layout()
    plt.show()


if __name__ == '__main__':
    main()
