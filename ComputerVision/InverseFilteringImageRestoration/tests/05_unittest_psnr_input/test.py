import numpy as np

from deconvolution import compute_psnr
from numpy.testing import assert_allclose


def test_psnr():
    img_gt = np.array([
        [1, 2, 3],
        [4, 0, 6],
        [7, 8, 9]
    ])
    img_pred = np.array([
        [1, 2, 3],
        [4, 5, 6],
        [7, 8, 9]
    ])
    assert_allclose(compute_psnr(img_pred, img_gt), 43.69383, rtol=0, atol=1e-3)


if __name__ == '__main__':
    test_psnr()
