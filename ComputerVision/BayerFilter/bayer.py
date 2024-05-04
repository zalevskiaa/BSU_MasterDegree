import numpy as np
import skimage


def get_bayer_masks(n_rows: int, n_cols: int):
    r = np.tile(np.array([[0, 1], [0, 0]], dtype=bool), (n_rows // 2 + 1, n_cols // 2 + 1))[:n_rows, :n_cols]
    g = np.tile(np.array([[1, 0], [0, 1]], dtype=bool), (n_rows // 2 + 1, n_cols // 2 + 1))[:n_rows, :n_cols]
    b = np.tile(np.array([[0, 0], [1, 0]], dtype=bool), (n_rows // 2 + 1, n_cols // 2 + 1))[:n_rows, :n_cols]
    return np.dstack((r, g, b))


def get_colored_img(raw_img):
    masks = get_bayer_masks(*raw_img.shape)
    r = masks[:, :, 0] * raw_img
    g = masks[:, :, 1] * raw_img
    b = masks[:, :, 2] * raw_img

    return np.dstack((r, g, b))


def bilinear_interpolation(colored_img):
    masks = get_bayer_masks(colored_img.shape[0], colored_img.shape[1])

    result = np.zeros(colored_img.shape)
    for row in range(1, colored_img.shape[0] - 1):
        for col in range(1, colored_img.shape[1] - 1):
            w_mask = masks[row - 1:row + 2, col - 1:col + 2, :]
            w_img = colored_img[row - 1:row + 2, col - 1:col + 2, :]
            res = (w_mask * w_img).sum(axis=(0, 1)) / w_mask.sum(axis=(0, 1))
            for color in range(colored_img.shape[2]):
                result[row, col, color] = colored_img[row, col, color] if masks[row, col, color] else res[color]
    return result / 255


def improved_interpolation(raw_img):
    masks = get_bayer_masks(*raw_img.shape)
    colored_img = get_colored_img(raw_img)

    g_r_coeff = np.array([[0, 0, -1, 0, 0],
                          [0, 0, 2, 0, 0],
                          [-1, 2, 4, 2, -1],
                          [0, 0, 2, 0, 0],
                          [0, 0, -1, 0, 0],
    ], dtype=float) / 8
    r_g_r_coeff = np.array([[0, 0, 1/2, 0, 0],
                            [0, -1, 0, -1, 0],
                            [-1, 4, 5, 4, -1],
                            [0, -1, 0, -1, 0],
                            [0, 0, 1/2, 0, 0],
    ], dtype=float) / 8
    r_g_b_coeff = r_g_r_coeff.T
    r_b_coeff = np.array([[0, 0, -3/2, 0, 0],
                          [0, 2, 0, 2, 0],
                          [-3/2, 0, 6, 0, -3/2],
                          [0, 2, 0, 2, 0],
                          [0, 0, -3/2, 0, 0],
    ], dtype=float) / 8

    result = colored_img.astype(int)
    for row in range(2, raw_img.shape[0] - 2):
        for col in range(2, raw_img.shape[1] - 2):
            if masks[row, col, 0]:
                result[row, col, 1] = int((g_r_coeff * raw_img[row-2:row+3, col-2:col+3]).sum())    # G at R
                result[row, col, 2] = int((r_b_coeff * raw_img[row-2:row+3, col-2:col+3]).sum())    # B at R
                continue

            if masks[row, col, 1]:
                if masks[row, col-1, 0]:
                    result[row, col, 0] = int((r_g_r_coeff * raw_img[row-2:row+3, col-2:col+3]).sum())  # R at G in R row
                    result[row, col, 2] = int((r_g_b_coeff * raw_img[row-2:row+3, col-2:col+3]).sum())  # B at G in R row
                else:
                    result[row, col, 0] = int((r_g_b_coeff * raw_img[row-2:row+3, col-2:col+3]).sum())  # R at G in B row
                    result[row, col, 2] = int((r_g_r_coeff * raw_img[row-2:row+3, col-2:col+3]).sum())  # B at G in B row
                continue

            if masks[row, col, 2]:
                result[row, col, 0] = int((r_b_coeff * raw_img[row-2:row+3, col-2:col+3]).sum())    # R at B
                result[row, col, 1] = int((g_r_coeff * raw_img[row-2:row+3, col-2:col+3]).sum())    # G at B

    return result.clip(0, 255).astype('uint8')


def compute_mse(img_1, img_2):
    assert img_1.shape == img_2.shape
    return ((img_1 - img_2) ** 2).sum() / (img_1.shape[0] * img_1.shape[1] * img_1.shape[2])


def compute_psnr(img_pred, img_gt):
    ip = img_pred.astype('float64')
    ig = img_gt.astype('float64')
    mse = compute_mse(ip, ig)
    if np.isclose(mse, 0):
        raise ValueError
    return 10 * np.log10((ig ** 2).max() / mse)


def main():
    raw_img = np.array([[202, 150, 137, 121, 195],
                     [94, 113, 217, 68, 248],
                     [208, 170, 109, 67, 22],
                     [20, 93, 222, 54, 50],
                     [254, 252, 10, 187, 203]], dtype='uint8')
    colored_img = get_colored_img(raw_img)
    bi = bilinear_interpolation(colored_img)


if __name__ == '__main__':
    main()



