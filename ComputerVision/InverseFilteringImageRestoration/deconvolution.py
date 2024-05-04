import numpy as np


def gaussian_kernel(size, sigma):
    """
    Построение ядра фильтра Гаусса.

    @param  size  int    размер фильтра (нечетный)
    @param  sigma float  параметр размытия
    @return numpy array  фильтр Гаусса размером size x size
    """
    result = np.zeros((size, size))
    m = size // 2
    for i in range(size):
        for j in range(size):
            r = np.sqrt((i - m) ** 2 + (j - m) ** 2)
            result[i, j] = np.exp(-(r ** 2) / (2 * sigma ** 2)) / (2 * np.pi * sigma ** 2)
    result /= result.sum()
    assert np.isclose(result.sum(), 1)
    return result


def fourier_transform(h, shape):
    """
    Получение Фурье-образа искажающей функции

    @param  h            numpy array  искажающая функция h (ядро свертки)
    @param  shape        list         требуемый размер образа
    @return numpy array  H            Фурье-образ искажающей функции h
    """
    assert len(shape) == 2
    assert h.shape[0] <= shape[0]
    assert h.shape[1] <= shape[1]
    result = np.zeros(shape)
    result[:h.shape[0], :h.shape[1]] = h[:, :]
    from scipy.fft import fftn
    return fftn(result)


def inverse_kernel(H, threshold=1e-10):
    """
    Получение H_inv

    @param  H            numpy array    Фурье-образ искажающей функции h
    @param  threshold    float          порог отсечения для избежания деления на 0
    @return numpy array  H_inv
    """
    return np.array([
        0 if np.absolute(elem) <= threshold else 1 / elem
        for elem in H.reshape(-1)
    ], dtype=H.dtype).reshape(H.shape)


def inverse_filtering(blurred_img, h, threshold=1e-10):
    """
    Метод инверсной фильтрации

    @param  blurred_img    numpy array  искаженное изображение
    @param  h              numpy array  искажающая функция
    @param  threshold      float        параметр получения H_inv
    @return numpy array                 восстановленное изображение
    """
    G = fourier_transform(blurred_img, blurred_img.shape)
    H = fourier_transform(h, blurred_img.shape)
    H_inv = inverse_kernel(H, threshold)
    F_hat = G * H_inv
    from scipy.fft import ifftn
    f_hat = ifftn(F_hat)
    return np.absolute(f_hat)


def wiener_filtering(blurred_img, h, K=0.0010101010101010027):
    """
    Винеровская фильтрация

    @param  blurred_img    numpy array  искаженное изображение
    @param  h              numpy array  искажающая функция
    @param  K              float        константа из выражения (8)
    @return numpy array                 восстановленное изображение
    """
    G = fourier_transform(blurred_img, blurred_img.shape)
    H = fourier_transform(h, blurred_img.shape)
    H_conj = np.conj(H)
    F_hat = H_conj / (H * H_conj + K) * G
    from scipy.fft import ifftn
    f_hat = ifftn(F_hat)
    return np.absolute(f_hat)


def compute_psnr(img1, img2):
    """
    PSNR metric

    @param  img1    numpy array   оригинальное изображение
    @param  img2    numpy array   искаженное изображение
    @return float   PSNR(img1, img2)
    """

    def compute_mse(img_1, img_2):
        assert img_1.shape == img_2.shape
        return ((img_1 - img_2) ** 2).sum() / (img_1.shape[0] * img_1.shape[1])

    return 20 * np.log10(255 / np.sqrt(compute_mse(img1, img2)))
