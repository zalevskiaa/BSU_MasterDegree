import os
import numpy as np
import matplotlib.pyplot as plt
from skimage.io import imread
from scipy.ndimage import gaussian_filter
from skimage.metrics import peak_signal_noise_ratio
# !Этих импортов достаточно для решения данного задания, нельзя использовать другие библиотеки!


def pca_compression(matrix, p):
    """ Сжатие изображения с помощью PCA
    Вход: двумерная матрица (одна цветовая компонента картинки), количество компонент
    Выход: собственные векторы и проекция матрицы на новое пр-во
    """
    
    # Your code here
    
    # Отцентруем каждую строчку матрицы
    matrix_means = matrix.mean(axis=1)
    m_cen = matrix - matrix_means.reshape(-1, 1)

    # Найдем матрицу ковариации
    m_cov = np.cov(m_cen)

    # Ищем собственные значения и собственные векторы матрицы ковариации, используйте linalg.eigh из numpy
    eig_val, eig_vec = np.linalg.eigh(m_cov)

    # Посчитаем количество найденных собственных векторов
    _ = eig_vec.shape[1]

    # Сортируем собственные значения в порядке убывания
    eig_val_sort_ind = np.argsort(eig_val)[::-1]

    # Сортируем собственные векторы согласно отсортированным собственным значениям
    # !Это все для того, чтобы мы производили проекцию в направлении максимальной дисперсии!
    eig_vec = eig_vec[:, eig_val_sort_ind]

    # Оставляем только p собственных векторов
    eig_vec = eig_vec[:, :p]

    # Проекция данных на новое пространство
    result = eig_vec.T @ m_cen
    
    return eig_vec, result, matrix_means


def pca_decompression(compressed):
    """ Разжатие изображения
    Вход: список кортежей из собственных векторов и проекций для каждой цветовой компоненты
    Выход: разжатое изображение
    """
    
    result_img = []
    for i, comp in enumerate(compressed):
        # Матрично умножаем собственные векторы на проекции и прибавляем среднее значение по строкам исходной матрицы
        # !Это следует из описанного в самом начале примера!

        # Your code here
        result_img.append(comp[0] @ comp[1] + comp[2].reshape(-1, 1))

    result_img = np.array(result_img, dtype=int).transpose(1, 2, 0).clip(0, 255)

    return result_img


def pca_visualize():
    plt.clf()
    img = imread('cat.jpg')
    if len(img.shape) == 3:
        img = img[..., :3]
    fig, axes = plt.subplots(3, 3)
    fig.set_figwidth(12)
    fig.set_figheight(12)

    for i, p in enumerate([1, 5, 10, 20, 50, 100, 150, 200, 256]):
        compressed = []
        for j in range(0, 3):
            # Your code here
            compressed.append(pca_compression(img[:, :, j], p))

        axes[i // 3, i % 3].imshow(pca_decompression(compressed))
        axes[i // 3, i % 3].set_title('Компонент: {}'.format(p))

    fig.savefig("pca_visualization.png")


def rgb2ycbcr(img):
    """ Переход из пр-ва RGB в пр-во YCbCr
    Вход: RGB изображение
    Выход: YCbCr изображение
    """
    A = np.array([
        [0.299, 0.587, 0.114],
        [-0.1687, -0.3313, 0.5],
        [0.5, -0.4187, -0.081]
    ])
    b = np.array([0, 128, 128])

    result = np.zeros(img.shape)
    for i in range(img.shape[0]):
        for j in range(img.shape[1]):
            result[i, j, :] = b + A @ img[i, j, :]
    return result


def ycbcr2rgb(img):
    """ Переход из пр-ва YCbCr в пр-во RGB
    Вход: YCbCr изображение
    Выход: RGB изображение
    """
    A = np.array([
        [1, 0, 1.402],
        [1, -0.34414, -0.71414],
        [1, 1.77, 0],
    ])
    b = np.array([0, -128, -128])

    result = np.zeros(img.shape)
    for i in range(img.shape[0]):
        for j in range(img.shape[1]):
            result[i, j, :] = A @ (img[i, j, :] + b)

    # lol
    # result -= result.min()
    # result /= result.max() / 255

    return np.clip(result, 0, 255).astype(int)


def get_gauss_1():
    plt.clf()
    rgb_img = imread('Lenna.png')
    if len(rgb_img.shape) == 3:
        rgb_img = rgb_img[..., :3]

    # Your code here
    ycbcr_img = rgb2ycbcr(rgb_img)
    ycbcr_img[..., 1:] = gaussian_filter(ycbcr_img[..., 1:], 10)
    res_rgb_img = ycbcr2rgb(ycbcr_img)
    plt.imshow(res_rgb_img)
    plt.savefig("gauss_1.png")


def get_gauss_2():
    plt.clf()
    rgb_img = imread('Lenna.png')
    if len(rgb_img.shape) == 3:
        rgb_img = rgb_img[..., :3]

    # Your code here
    ycbcr_img = rgb2ycbcr(rgb_img)
    ycbcr_img[..., 0] = gaussian_filter(ycbcr_img[..., 0], 10)
    res_rgb_img = ycbcr2rgb(ycbcr_img)

    plt.imshow(res_rgb_img)
    plt.savefig("gauss_2.png")


def downsampling(component):
    """Уменьшаем цветовые компоненты в 2 раза
    Вход: цветовая компонента размера [A, B, 1]
    Выход: цветовая компонента размера [A // 2, B // 2, 1]
    """
    
    # Your code here
    return gaussian_filter(component, 10)[::2, ::2]


def dct(block):
    """Дискретное косинусное преобразование
    Вход: блок размера 8x8
    Выход: блок размера 8x8 после ДКП
    """
    # Your code here
    assert block.shape == (8, 8)

    def alpha(u: int):
        return 1 / np.sqrt(2) if u == 0 else 1

    result = np.zeros((8, 8), dtype=float)
    for u in range(8):
        for v in range(8):
            result[u, v] = 1/4 * alpha(u) * alpha(v) * np.sum(
                block[x, y] * np.cos((2 * x + 1) * u * np.pi / 16) * np.cos((2 * y + 1) * v * np.pi / 16)
                for y in range(8)
                for x in range(8)
            )

    return result


# Матрица квантования яркости
y_quantization_matrix = np.array([
    [16, 11, 10, 16, 24, 40, 51, 61],
    [12, 12, 14, 19, 26, 58, 60, 55],
    [14, 13, 16, 24, 40, 57, 69, 56],
    [14, 17, 22, 29, 51, 87, 80, 62],
    [18, 22, 37, 56, 68, 109, 103, 77],
    [24, 35, 55, 64, 81, 104, 113, 92],
    [49, 64, 78, 87, 103, 121, 120, 101],
    [72, 92, 95, 98, 112, 100, 103, 99]
])

# Матрица квантования цвета
color_quantization_matrix = np.array([
    [17, 18, 24, 47, 99, 99, 99, 99],
    [18, 21, 26, 66, 99, 99, 99, 99],
    [24, 26, 56, 99, 99, 99, 99, 99],
    [47, 66, 99, 99, 99, 99, 99, 99],
    [99, 99, 99, 99, 99, 99, 99, 99],
    [99, 99, 99, 99, 99, 99, 99, 99],
    [99, 99, 99, 99, 99, 99, 99, 99],
    [99, 99, 99, 99, 99, 99, 99, 99]
])


def quantization(block, quantization_matrix):
    """Квантование
    Вход: блок размера 8x8 после применения ДКП; матрица квантования
    Выход: блок размера 8x8 после квантования. Округление осуществляем с помощью np.round
    """
    # Your code here
    return np.round(block / quantization_matrix)


def own_quantization_matrix(default_quantization_matrix, q):
    """Генерация матрицы квантования по Quality Factor
    Вход: "стандартная" матрица квантования; Quality Factor
    Выход: новая матрица квантования
    Hint: если после проделанных операций какие-то элементы обнулились, то замените их единицами
    """

    assert 1 <= q <= 100

    # Your code here
    s = 5000 / q if q < 5 else 200 - 2 * q if q <= 99 else 1

    result = ((default_quantization_matrix * s + 50) / 100).astype(int)
    result[result == 0] = 1

    return result


zigzag_index = np.array([0, 1, 8, 16, 9, 2, 3, 10,
                  17, 24, 32, 25, 18, 11, 4, 5,
                  12, 19, 26, 33, 40, 48, 41, 34,
                  27, 20, 13, 6, 7, 14, 21, 28,
                  35, 42, 49, 56, 57, 50, 43, 36,
                  29, 22, 15, 23, 30, 37, 44, 51,
                  58, 59, 52, 45, 38, 31, 39, 46,
                  53, 60, 61, 54, 47, 55, 62, 63])


def zigzag(block):
    """Зигзаг-сканирование
    Вход: блок размера 8x8
    Выход: список из элементов входного блока, получаемый после его обхода зигзаг-сканированием
    """
    
    # Your code here
    assert block.shape == (8, 8)

    return block.reshape(-1)[zigzag_index]


def compression(zigzag_list):
    """Сжатие последовательности после зигзаг-сканирования
    Вход: список после зигзаг-сканирования
    Выход: сжатый список в формате, который был приведен в качестве примера в самом начале данного пункта
    """

    # Your code here
    result = []
    zero_count = 0

    for elem in zigzag_list:
        if elem == 0:
            zero_count += 1
        else:
            if zero_count > 0:
                result.append(0)
                result.append(zero_count)
                zero_count = 0
            result.append(elem)

    if zero_count > 0:
        result.append(0)
        result.append(zero_count)

    return result


def jpeg_compression(img, quantization_matrixes):
    """JPEG-сжатие
    Вход: цветная картинка, список из 2-ух матриц квантования
    Выход: список списков со сжатыми векторами: [[compressed_y1,...], [compressed_Cb1,...], [compressed_Cr1,...]]
    """

    # Your code here
    
    # Переходим из RGB в YCbCr
    ecbcr_img = rgb2ycbcr(img)

    # Уменьшаем цветовые компоненты
    y_component = ecbcr_img[:, :, 0]
    cb_component = downsampling(ecbcr_img[..., 1])
    cr_component = downsampling(ecbcr_img[..., 2])

    # Делим все компоненты на блоки 8x8 и все элементы блоков переводим из [0, 255] в [-128, 127]
    def component_to_blocks(component):
        bs = 8
        block = []

        for i in range(0, component.shape[0], bs):
            if i + bs > component.shape[0]:
                break
            for j in range(0, component.shape[1], bs):
                if j + bs > component.shape[1]:
                    break
                block.append(component[i:i + bs, j:j + bs] - 128)

        return block

    blocks = []
    blocks.append(component_to_blocks(y_component))
    blocks.append(component_to_blocks(cb_component))
    blocks.append(component_to_blocks(cr_component))

    # Применяем ДКП, квантование, зизгаз-сканирование и сжатие
    result = [[], [], []]
    for block in blocks[0]:
        block = dct(block)
        block = quantization(block, quantization_matrixes[0])
        block = zigzag(block)
        block = compression(block)
        result[0].append(block)

    for color in range(1, 3):
        for block in blocks[color]:
            block = dct(block)
            block = quantization(block, quantization_matrixes[1])
            block = zigzag(block)
            block = compression(block)
            result[color].append(block)

    return result


def inverse_compression(compressed_list):
    """Разжатие последовательности
    Вход: сжатый список
    Выход: разжатый список
    """
    
    # Your code here
    result = []
    it = iter(compressed_list)
    while True:
        try:
            elem = next(it)
            if elem == 0:
                for i in range(next(it)):
                    result.append(0)
            else:
                result.append(elem)
        except StopIteration:
            break

    return result


inverse_zigzag_index = np.argsort(zigzag_index)


def inverse_zigzag(input):
    """Обратное зигзаг-сканирование
    Вход: список элементов
    Выход: блок размера 8x8 из элементов входного списка, расставленных в матрице в порядке их следования в зигзаг-сканировании
    """

    # Your code here
    
    return np.array(input)[inverse_zigzag_index].reshape(8, 8)


def inverse_quantization(block, quantization_matrix):
    """Обратное квантование
    Вход: блок размера 8x8 после применения обратного зигзаг-сканирования; матрица квантования
    Выход: блок размера 8x8 после квантования. Округление не производится
    """
    
    # Your code here
    
    return block * quantization_matrix


def inverse_dct(block):
    """Обратное дискретное косинусное преобразование
    Вход: блок размера 8x8
    Выход: блок размера 8x8 после обратного ДКП. Округление осуществляем с помощью np.round
    """

    # Your code here

    assert block.shape == (8, 8)

    def alpha(u: int):
        return 1 / np.sqrt(2) if u == 0 else 1

    result = np.zeros((8, 8), dtype=float)
    for x in range(8):
        for y in range(8):
            result[x, y] = 1 / 4 * np.sum(
                alpha(u) * alpha(v) * block[u, v] * np.cos((2 * x + 1) * u * np.pi / 16) * np.cos((2 * y + 1) * v * np.pi / 16)
                for v in range(8)
                for u in range(8)
            )

    return np.round(result)


def upsampling(component):
    """Увеличиваем цветовые компоненты в 2 раза
    Вход: цветовая компонента размера [A, B, 1]
    Выход: цветовая компонента размера [2 * A, 2 * B, 1]
    """

    # Your code here
    s = component.shape
    assert len(s) == 2

    result = np.zeros((2 * s[0], 2 * s[1]))

    result[::2, ::2] = component[:, :]
    result[1::2, ::2] = component[:, :]
    result[::2, 1::2] = component[:, :]
    result[1::2, 1::2] = component[:, :]

    return result


def jpeg_decompression(result, result_shape, quantization_matrixes):
    """Разжатие изображения
    Вход: result список сжатых данных, размер ответа, список из 2-ух матриц квантования
    Выход: разжатое изображение
    """

    # Your code here
    # 1
    blocks = [[], [], []]
    for block in result[0]:
        block = inverse_compression(block)
        block = inverse_zigzag(block)
        block = inverse_quantization(block, quantization_matrixes[0])
        block = inverse_dct(block)
        blocks[0].append(block)

    for color in range(1, 3):
        for block in result[color]:
            block = inverse_compression(block)
            block = inverse_zigzag(block)
            block = inverse_quantization(block, quantization_matrixes[1])
            block = inverse_dct(block)
            blocks[color].append(block)

    # 2
    def blocks_to_component(blocks, component_shape):
        bs = 8
        blocks_shape = component_shape[0] // bs, component_shape[1] // bs
        assert blocks_shape[0] * bs == component_shape[0]
        assert blocks_shape[1] * bs == component_shape[1]

        component = np.zeros((component_shape))
        it = iter(blocks)
        for i in range(blocks_shape[0]):
            for j in range(blocks_shape[1]):
                component[bs*i:bs*i+bs, bs*j:bs*j+bs] = np.array(next(it)) + 128

        return component

    component_shape = np.array(result_shape)[:2]
    y_component = blocks_to_component(blocks[0], component_shape)
    cb_component = blocks_to_component(blocks[1], component_shape // 2)
    cr_component = blocks_to_component(blocks[2], component_shape // 2)

    # 3
    cb_component = upsampling(cb_component)
    cr_component = upsampling(cr_component)
    assert y_component.shape == cb_component.shape == cr_component.shape

    # 4
    ycbcr_img = np.zeros((*y_component.shape, 3))
    ycbcr_img[..., 0] = y_component
    ycbcr_img[..., 1] = cb_component
    ycbcr_img[..., 2] = cr_component

    # 5
    rgb_img = ycbcr2rgb(ycbcr_img)
    assert rgb_img.shape == result_shape

    return rgb_img


def jpeg_visualize():
    plt.clf()
    img = imread('Lenna.png')
    if len(img.shape) == 3:
        img = img[..., :3]
    fig, axes = plt.subplots(2, 3)
    fig.set_figwidth(12)
    fig.set_figheight(12)

    for i, p in enumerate([1, 10, 20, 50, 80, 100]):
        matrixes = [
            own_quantization_matrix(y_quantization_matrix, p),
            own_quantization_matrix(color_quantization_matrix, p),
        ]
        compressed = jpeg_compression(img, matrixes)
        decompressed = jpeg_decompression(compressed, img.shape, matrixes)

        axes[i // 3, i % 3].imshow(decompressed)
        axes[i // 3, i % 3].set_title('Quality Factor: {}'.format(p))

    fig.savefig("jpeg_visualization.png")


def compression_pipeline(img, c_type, param=1):
    """Pipeline для PCA и JPEG
    Вход: исходное изображение; название метода - 'pca', 'jpeg'; 
    param - кол-во компонент в случае PCA, и Quality Factor для JPEG
    Выход: изображение; количество бит на пиксель
    """
    
    assert c_type.lower() == 'jpeg' or c_type.lower() == 'pca'
    
    if c_type.lower() == 'jpeg':
        y_quantization = own_quantization_matrix(y_quantization_matrix, param)
        color_quantization = own_quantization_matrix(color_quantization_matrix, param)
        matrixes = [y_quantization, color_quantization]
        
        compressed = jpeg_compression(img, matrixes)
        img = jpeg_decompression(compressed, img.shape, matrixes)
    elif c_type.lower() == 'pca':
        compressed = []
        for j in range(0, 3):
            compressed.append((pca_compression(img[:, :, j].astype(np.float64).copy(), param)))
            
        img = pca_decompression(compressed)
        compressed.extend([np.mean(img[:, :, 0], axis=1), np.mean(img[:, :, 1], axis=1), np.mean(img[:, :, 2], axis=1)])
        
    if 'tmp' not in os.listdir() or not os.path.isdir('tmp'):
        os.mkdir('tmp')
        
    np.savez_compressed(os.path.join('tmp', 'tmp.npz'), compressed)
    size = os.stat(os.path.join('tmp', 'tmp.npz')).st_size * 8
    os.remove(os.path.join('tmp', 'tmp.npz'))
        
    return img, size / (img.shape[0] * img.shape[1])


def calc_metrics(img_path, c_type, param_list):
    """Подсчет PSNR и Rate-Distortion для PCA и JPEG. Построение графиков
    Вход: пусть до изображения; тип сжатия; список параметров: кол-во компонент в случае PCA, и Quality Factor для JPEG
    """
    
    assert c_type.lower() == 'jpeg' or c_type.lower() == 'pca'
    
    img = imread(img_path)
    if len(img.shape) == 3:
        img = img[..., :3]
    
    outputs = []
    for param in param_list:
        outputs.append(compression_pipeline(img.copy(), c_type, param))
     
    psnr = [peak_signal_noise_ratio(img, output[0]) for output in outputs]
    rate = [output[1] for output in outputs]
    
    fig, (ax1, ax2) = plt.subplots(nrows=1, ncols=2)
    fig.set_figwidth(20)
    fig.set_figheight(5)
    
    ax1.set_title('PSNR for {}'.format(c_type.upper()))
    ax1.plot(param_list, psnr, 'tab:orange')
    ax1.set_xlabel('Quality Factor')
    ax1.set_ylabel('PSNR')
    
    ax2.set_title('Rate-Distortion for {}'.format(c_type.upper()))
    ax2.plot(psnr, rate, 'tab:red')
    ax2.set_xlabel('Distortion')
    ax2.set_ylabel('Rate')
    return fig


def get_pca_metrics_graph():
    plt.clf()
    fig = calc_metrics('Lenna.png', 'pca', [1, 5, 10, 20, 50, 100, 150, 200, 256])
    fig.savefig("pca_metrics_graph.png")


def get_jpeg_metrics_graph():
    plt.clf()
    fig = calc_metrics('Lenna.png', 'jpeg', [1, 10, 20, 50, 80, 100])
    fig.savefig("jpeg_metrics_graph.png")


def main():
    get_gauss_1()
    get_gauss_2()
    #jpeg_visualize()
    #get_pca_metrics_graph()
    #get_jpeg_metrics_graph()
    return


if __name__ == '__main__':
    main()
