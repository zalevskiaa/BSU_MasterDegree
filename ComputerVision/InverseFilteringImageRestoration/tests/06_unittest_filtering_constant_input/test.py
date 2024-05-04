import os
import numpy as np

from deconvolution import wiener_filtering, compute_psnr, gaussian_kernel


def test_filtering_constant():
    dirname = os.path.dirname(__file__)
    original_img = np.load(os.path.join(dirname, 'original_img.npy'))
    noisy_img = np.load(os.path.join(dirname, 'noisy_img.npy'))

    kernel = gaussian_kernel(size=15, sigma=5)
    filtered_img = wiener_filtering(noisy_img, kernel)

    assert compute_psnr(filtered_img, original_img) - compute_psnr(noisy_img, original_img) > 7.0


if __name__ == '__main__':
    test_filtering_constant()
