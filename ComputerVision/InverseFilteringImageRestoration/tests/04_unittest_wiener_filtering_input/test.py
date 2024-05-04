import numpy as np

from numpy.testing import assert_allclose
from deconvolution import wiener_filtering


def test_wiener_filtering():
    img = np.array([
        [1, 2, 3],
        [4, 5, 6],
        [7, 8, 9]
    ])
    kernel = np.array([[1]])
    assert_allclose(wiener_filtering(img, kernel, K=0), img, rtol=0, atol=1e-3)

    blurred_img = np.array([
        [0.4, 0.4, 0.6],
        [0.4, 0.4, 0.6],
        [0.6, 0.6, 1.]
    ])
    kernel = np.array([
        [0,   0.2, 0],
        [0.2, 0.2, 0.2],
        [0,   0.2, 0]
    ])
    restored_img = np.array([
        [0, 1, 0],
        [1, 1, 1],
        [0, 1, 0]
    ])
    assert_allclose(wiener_filtering(blurred_img, kernel, K=0), restored_img, rtol=0, atol=1e-3)


if __name__ == '__main__':
    test_wiener_filtering()
