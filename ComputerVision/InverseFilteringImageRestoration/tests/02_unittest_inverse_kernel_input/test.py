import numpy as np

from numpy.testing import assert_allclose
from deconvolution import inverse_kernel


def test_inverse_kernel():
    kernels = []
    answers = []

    kernels.append([
        [10, 1,  100],
        [1,  2,  3],
        [5,  10, 10]
    ])
    answers.append([
        [0.1, 0,   0.01],
        [0,   0,   0],
        [0,   0.1, 0.1]
    ])

    kernels.append([
        [10.,   -10.j, 100, 2.j],
        [2.j,   5.j,   0,   100.],
        [100.j, 1.j,   10., 5. - 10.j]
    ])
    answers.append([
        [0.1,    0.1j, 0.01, 0.],
        [0.,     0.,   0.,   0.01],
        [-0.01j, 0.,   0.1,  0.04 + 0.08j]
    ])

    kernels = [np.array(kernel, dtype=np.complex64) for kernel in kernels]
    answers = map(np.array, answers)
    for kernel, answer in zip(kernels, answers):
        assert_allclose(inverse_kernel(kernel, threshold=5), answer, rtol=0, atol=1e-3)


if __name__ == '__main__':
    test_inverse_kernel()
