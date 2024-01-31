import numpy as np


def sigmoid(x, deriv=False):
    """
    Represents the non-linear function where `deriv` computes the derivative
    of this function.
    """
    if deriv is True:
        return sigmoid(x) / (1 - sigmoid(x))

    return 1 / (1 + np.exp(-x))


class Perceptron:
    """
    A class representing a three-layer perceptron.
    """

    def __init__(self, n, h, m, beta=0.1, alpha=0.1):
        """
        Initializes a `Perceptron` instance with the following parameters:

        - `n` — number of input neurons;
        - `h` — number of hidden neurons;
        - `m` — number of output neurons;
        - `beta` — convergence step for the hidden layer;
        - `alpha` — convergence step for the output layer.
        """
        self.v = 2 * np.random.random((h, n)) - 1
        self.q = 2 * np.random.random((h, 1)) - 1

        self.w = 2 * np.random.random((m, h)) - 1
        self.t = 2 * np.random.random((m, 1)) - 1

        self.beta = beta
        self.alpha = alpha

    def feed_forward(self, x):
        """
        Returns the response of the perceptron to `x`.
        """
        g = sigmoid(np.dot(self.v, x) + self.q)
        y = sigmoid(np.dot(self.w, g) + self.t)

        return y

    def gradient_descent(self, training_data):
        """
        Computes negative gradient and applies it to the weights and biases of the perceptron using the stochastic gradizent descent method.
        """
        nabla_w = np.zeros(self.w.shape)
        nabla_t = np.zeros(self.t.shape)

        nabla_v = np.zeros(self.v.shape)
        nabla_q = np.zeros(self.q.shape)

        n = len(training_data)

        for x, y in training_data:
            w, t, v, q = self.back_propagation(x, y)

            nabla_w += w
            nabla_t += t

            nabla_v += v
            nabla_q += q

        self.w -= self.alpha * nabla_w / n
        self.t -= self.alpha * nabla_t / n

        self.v -= self.beta * nabla_v / n
        self.q -= self.beta * nabla_q / n

    def back_propagation(self, x, y):
        """
        Computes gradient components of `w`, `t`, `v` and `q` using the back
        propagation method.
        """
        current_g = sigmoid(np.dot(self.v, x) + self.q)
        current_y = sigmoid(np.dot(self.w, current_g) + self.t)

        delta_y = 2 * (current_y - y) * sigmoid(current_y, deriv=True)

        nabla_w = np.dot(delta_y, current_g.T)
        nabla_t = delta_y

        delta_g = np.dot(self.w.T, delta_y) * sigmoid(current_g, deriv=True)

        nabla_v = np.dot(delta_g, x.T)
        nabla_q = delta_g

        return (nabla_w, nabla_t, nabla_v, nabla_q)
