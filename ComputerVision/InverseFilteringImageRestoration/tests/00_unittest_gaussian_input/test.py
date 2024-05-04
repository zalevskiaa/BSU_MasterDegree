import numpy as np

from numpy.testing import assert_allclose
from deconvolution import gaussian_kernel


def test_gaussian():
    answer = np.array([
        [0.00296902, 0.0133062, 0.02193823, 0.0133062, 0.00296902],
        [0.01330621, 0.0596343, 0.09832033, 0.0596343, 0.01330621],
        [0.02193823, 0.0983203, 0.16210282, 0.0983203, 0.02193823],
        [0.01330621, 0.0596343, 0.09832033, 0.0596343, 0.01330621],
        [0.00296902, 0.0133062, 0.02193823, 0.0133062, 0.00296902]
    ])

    kernel = gaussian_kernel(size=5, sigma=1)
    assert_allclose(kernel, answer, rtol=0, atol=1e-3)


if __name__ == '__main__':
    test_gaussian()



