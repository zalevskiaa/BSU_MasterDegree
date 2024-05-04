import numpy as np

from numpy.testing import assert_allclose
from deconvolution import fourier_transform


def test_fourier_transform():
    kernels = []
    shapes = []
    answers = []

    kernels.append([[1]])
    shapes.append([3, 3])
    answers.append([
        [1, 1, 1],
        [1, 1, 1],
        [1, 1, 1]
    ])

    kernels.append([[1, 1], [1, 1]])
    shapes.append([4, 4])
    answers.append([
        [4. + 0.j, 2. - 2.j, 0. + 0.j, 2. + 2.j],
        [2. - 2.j, 0. - 2.j, 0. + 0.j, 2. + 0.j],
        [0. + 0.j, 0. + 0.j, 0. + 0.j, 0. + 0.j],
        [2. + 2.j, 2. + 0.j, 0. + 0.j, 0. + 2.j]
    ])

    kernels = map(np.array, kernels)
    answers = [np.array(answer, dtype=np.complex64) for answer in answers]
    for kernel, shape, answer in zip(kernels, shapes, answers):
        assert_allclose(fourier_transform(kernel, shape), answer, rtol=0, atol=1e-3)


if __name__ == '__main__':
    test_fourier_transform()



