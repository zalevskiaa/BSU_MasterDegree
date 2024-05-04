import os
import numpy as np

from numpy.testing import assert_allclose
from deconvolution import inverse_filtering


def test_inverse_filtering():
    img = np.array([
        [1, 2, 3],
        [4, 5, 6],
        [7, 8, 9]
    ])
    kernel = np.array([[1]])
    threshold = 0
    assert_allclose(inverse_filtering(img, kernel, threshold), img, rtol=0, atol=1e-3)

    dirname = os.path.dirname(__file__)
    original_img = np.load(os.path.join(dirname, 'original_img.npy'))
    blurred_img = np.load(os.path.join(dirname, 'blurred_img.npy'))
    kernel = np.load(os.path.join(dirname, 'kernel.npy'))
    threshold = 1e-10

    restored_img = inverse_filtering(blurred_img, kernel, threshold)
    mse = np.mean((restored_img - original_img) ** 2)
    assert mse < 1e-3


if __name__ == '__main__':
    test_inverse_filtering()
