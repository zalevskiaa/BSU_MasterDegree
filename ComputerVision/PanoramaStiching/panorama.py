import numpy as np
from skimage.feature import ORB, match_descriptors
from skimage.color import rgb2gray
from skimage.transform import ProjectiveTransform
from skimage.transform import warp
from skimage.filters import gaussian
from numpy.linalg import inv

DEFAULT_TRANSFORM = ProjectiveTransform


def find_orb(img, n_keypoints=250):
    """Find keypoints and their descriptors in image.

    img ((W, H, 3)  np.ndarray) : 3-channel image
    n_keypoints (int) : number of keypoints to find

    Returns:
        (N, 2)  np.ndarray : keypoints
        (N, 256)  np.ndarray, type=np.bool  : descriptors
    """

    # your code here
    # https://scikit-image.org/docs/stable/auto_examples/features_detection/plot_orb.html
    img = rgb2gray(img)

    descriptor_extractor = ORB(n_keypoints=n_keypoints)
    descriptor_extractor.detect_and_extract(img)

    keypoints = descriptor_extractor.keypoints
    descriptors = descriptor_extractor.descriptors

    return keypoints, descriptors


def center_and_normalize_points(points):
    """Center the image points, such that the new coordinate system has its
    origin at the centroid of the image points.

    Normalize the image points, such that the mean distance from the points
    to the origin of the coordinate system is sqrt(2).

    points ((N, 2) np.ndarray) : the coordinates of the image points

    Returns:
        (3, 3) np.ndarray : the transformation matrix to obtain the new points
        (N, 2) np.ndarray : the transformed image points
    """

    pointsh = np.column_stack([points, np.ones((points.shape[0], 1))]).T

    # your code here
    C_x, C_y = points.mean(0)

    N = np.sqrt(2) / (
        np.sqrt(((pointsh[0] - C_x) ** 2) + ((pointsh[1] - C_y) ** 2))
    ).mean()

    transformation_matrix = np.array([
        [N, 0, -N * C_x],
        [0, N, -N * C_y],
        [0, 0, 1],
    ])

    return transformation_matrix, (transformation_matrix @ pointsh).T


def find_homography(src_keypoints, dest_keypoints):
    """Estimate homography matrix from two sets of N (4+) corresponding points.

    src_keypoints ((N, 2) np.ndarray) : source coordinates
    dest_keypoints ((N, 2) np.ndarray) : destination coordinates

    Returns:
        ((3, 3) np.ndarray) : homography matrix
    """

    src_matrix, src = center_and_normalize_points(src_keypoints)
    dest_matrix, dest = center_and_normalize_points(dest_keypoints)

    H = np.zeros((3, 3))

    # your code here
    A = []
    for i in range(src_keypoints.shape[0]):
        A.append([
            -src[i][0], -src[i][1], -1,
            0, 0, 0,
            dest[i][0] * src[i][0], dest[i][0] * src[i][1], dest[i][0]
        ])
        A.append([
            0, 0, 0,
            -src[i][0], -src[i][1], -1,
            dest[i][1] * src[i][0], dest[i][1] * src[i][1], dest[i][1]
        ])
    A = np.array(A)

    _, _, V = np.linalg.svd(A, full_matrices=True)
    H[0], H[1], H[2] = V[-1, :3], V[-1, 3:6], V[-1, 6:]
    H = inv(dest_matrix) @ H @ src_matrix

    return H


def ransac_transform(src_keypoints, src_descriptors, dest_keypoints, dest_descriptors, max_trials=100, residual_threshold=2, return_matches=False):
    """Match keypoints of 2 images and find ProjectiveTransform using RANSAC algorithm.

    src_keypoints ((N, 2) np.ndarray) : source coordinates
    src_descriptors ((N, 256) np.ndarray) : source descriptors
    dest_keypoints ((N, 2) np.ndarray) : destination coordinates
    dest_descriptors ((N, 256) np.ndarray) : destination descriptors
    max_trials (int) : maximum number of iterations for random sample selection.
    residual_threshold (float) : maximum distance for a data point to be classified as an inlier.
    return_matches (bool) : if True function returns matches

    Returns:
        skimage.transform.ProjectiveTransform : transform of source image to destination image
        (Optional)(N, 2) np.ndarray : inliers' indexes of source and destination images
    """

    # your code here
    # https://scikit-image.org/docs/stable/api/skimage.feature.html#skimage.feature.match_descriptors
    inds_match = match_descriptors(src_descriptors, dest_descriptors)

    src_keypoints, dest_keypoints = src_keypoints[inds_match[:, 0]], dest_keypoints[inds_match[:, 1]]

    best_count = 0
    best_indices = []

    for i in range(max_trials):
        # a
        indices = np.random.choice(src_keypoints.shape[0], 4, replace=False)

        # b
        H = find_homography(src_keypoints[indices], dest_keypoints[indices])

        # c
        # https://scikit-image.org/docs/stable/api/skimage.transform.html#skimage.transform.ProjectiveTransform
        H_dest_keypoints = ProjectiveTransform(H)(src_keypoints)

        indices = ((dest_keypoints - H_dest_keypoints) ** 2).sum(axis=1) < residual_threshold
        count = indices.sum()

        # d
        if count > best_count:
            best_count, best_indices = count, indices

    assert best_count > 0
    result = ProjectiveTransform(find_homography(src_keypoints[best_indices], dest_keypoints[best_indices]))

    if return_matches:
        return result, inds_match[best_indices]

    return result


def find_simple_center_warps(forward_transforms):
    """Find transformations that transform each image to plane of the central image.

    forward_transforms (Tuple[N]) : - pairwise transformations

    Returns:
        Tuple[N + 1] : transformations to the plane of central image
    """
    image_count = len(forward_transforms) + 1
    center_index = (image_count - 1) // 2

    result = [None] * image_count
    result[center_index] = DEFAULT_TRANSFORM()

    # your code here
    for i in range(center_index - 1, -1, -1):
        result[i] = result[i + 1] + ProjectiveTransform(forward_transforms[i].params)
    for i in range(center_index + 1, image_count):
        result[i] = result[i - 1] + ProjectiveTransform(inv(forward_transforms[i - 1].params))

    return tuple(result)


def get_corners(image_collection, center_warps):
    """Get corners' coordinates after transformation."""
    for img, transform in zip(image_collection, center_warps):
        height, width, _ = img.shape
        corners = np.array([[0, 0],
                            [height, 0],
                            [height, width],
                            [0, width]])

        yield transform(corners)[:, ::-1]


def get_min_max_coords(corners):
    """Get minimum and maximum coordinates of corners."""
    corners = np.concatenate(corners)
    return corners.min(axis=0), corners.max(axis=0)


def get_final_center_warps(image_collection, simple_center_warps):
    """Find final transformations.

        image_collection (Tuple[N]) : list of all images
        simple_center_warps (Tuple[N])  : transformations unadjusted for shift

        Returns:
            Tuple[N] : final transformations
        """
    # your code here
    min_coords, max_coords = get_min_max_coords(
        list(get_corners(image_collection, simple_center_warps))
    )

    shift = np.asarray([[1, 0, -min_coords[1]],
                        [0, 1, -min_coords[0]],
                        [0, 0, 1]])

    final_center_warps = tuple([
        ProjectiveTransform(shift @ scw.params)
        for scw in simple_center_warps
    ])
    output_shape = (
        int((max_coords[1] - min_coords[1])),
        int((max_coords[0] - min_coords[0])),
    )
    return final_center_warps, output_shape


def rotate_transform_matrix(transform):
    """Rotate matrix so it can be applied to row:col coordinates."""
    matrix = transform.params[(1, 0, 2), :][:, (1, 0, 2)]
    return type(transform)(matrix)


def warp_image(image, transform, output_shape):
    """Apply transformation to an image and its mask

    image ((W, H, 3)  np.ndarray) : image for transformation
    transform (skimage.transform.ProjectiveTransform): transformation to apply
    output_shape (int, int) : shape of the final pano

    Returns:
        (W, H, 3)  np.ndarray : warped image
        (W, H)  np.ndarray : warped mask
    """
    # your code here
    warped_image = warp(image, ProjectiveTransform(inv(rotate_transform_matrix(transform).params)),
                        output_shape=output_shape)
    warped_mask = (warped_image != 0).any(axis=2)
    return warped_image, warped_mask


def merge_pano(image_collection, final_center_warps, output_shape):
    """ Merge the whole panorama

    image_collection (Tuple[N]) : list of all images
    final_center_warps (Tuple[N])  : transformations
    output_shape (int, int) : shape of the final pano

    Returns:
        (output_shape) np.ndarray: final pano
    """
    result = np.zeros(output_shape + (3,))
    result_mask = np.zeros(output_shape, dtype=np.bool8)

    # your code here
    for ic, fcw in zip(image_collection, final_center_warps):
        image, mask = warp_image(ic, fcw, output_shape)

        for i in range(output_shape[0]):
            for j in range(output_shape[1]):
                if result_mask[i, j]:
                    continue
                if mask[i, j]:
                    result_mask[i, j] = True
                    result[i, j] = image[i, j]

    return result


def get_gaussian_pyramid(image, n_layers, sigma):
    """Get Gaussian pyramid.

    image ((W, H, 3)  np.ndarray) : original image
    n_layers (int) : number of layers in Gaussian pyramid
    sigma (int) : Gaussian sigma

    Returns:
        tuple(n_layers) Gaussian pyramid

    """
    # your code here
    image_copy = image.copy()
    layers = [image_copy]

    for i in range(1, n_layers):
        image_copy = gaussian(image_copy, sigma)
        layers.append(image_copy)

    return tuple(layers)


def get_laplacian_pyramid(image, n_layers=5, sigma=5):
    """Get Laplacian pyramid

    image ((W, H, 3)  np.ndarray) : original image
    n_layers (int) : number of layers in Laplacian pyramid
    sigma (int) : Gaussian sigma

    Returns:
        tuple(n_layers) Laplacian pyramid
    """
    # your code here
    gaussian_pyramid = get_gaussian_pyramid(image, n_layers, sigma)

    layers = [
        gaussian_pyramid[i] - gaussian_pyramid[i + 1]
        for i in range(n_layers - 1)
    ] + [gaussian_pyramid[-1]]

    return tuple(layers)


def merge_laplacian_pyramid(laplacian_pyramid):
    """Recreate original image from Laplacian pyramid

    laplacian pyramid: tuple of np.array (h, w, 3)

    Returns:
        np.array (h, w, 3)
    """
    return sum(laplacian_pyramid)


def increase_contrast(image_collection):
    """Increase contrast of the images in collection"""
    result = []

    for img in image_collection:
        img = img.copy()
        for i in range(img.shape[-1]):
            img[:, :, i] -= img[:, :, i].min()
            img[:, :, i] /= img[:, :, i].max()
        result.append(img)

    return result


def gaussian_merge_pano(image_collection, final_center_warps, output_shape, n_layers=5, image_sigma=2, merge_sigma=1):
    """ Merge the whole panorama using Laplacian pyramid

    image_collection (Tuple[N]) : list of all images
    final_center_warps (Tuple[N])  : transformations
    output_shape (int, int) : shape of the final pano
    n_layers (int) : number of layers in Laplacian pyramid
    image_sigma (int) :  sigma for Gaussian filter for images
    merge_sigma (int) : sigma for Gaussian filter for masks

    Returns:
        (output_shape) np.ndarray: final pano
    """
    # your code here
    result_image = np.zeros(output_shape + (3,))
    result_mask = np.zeros(output_shape, dtype=bool)

    for i, img in enumerate(image_collection):
        warped_image, warped_mask = warp_image(img, final_center_warps[i], output_shape)
        if i == 0:
            result_image += warped_image
            result_mask += warped_mask
            continue

        intersection = result_mask & warped_mask
        mid = [j for j, elem in enumerate(intersection[intersection.shape[0] // 2, :]) if elem != 0]
        mid_j = (mid[0] + mid[-1]) // 2

        left_half = np.copy(intersection)
        left_half[:, mid_j:] = 0
        right_half = np.copy(intersection)
        right_half[:, :mid_j] = 0

        gaussian_pyramid = get_gaussian_pyramid(
            (result_mask * (~right_half)).astype(float), n_layers, merge_sigma
        )
        laplacian_pyramid_1 = get_laplacian_pyramid(result_image, n_layers, image_sigma)
        laplacian_pyramid_2 = get_laplacian_pyramid(warped_image, n_layers, image_sigma)

        result_image = sum([
            lp1 * gp[:, :, None] + (1 - gp)[:, :, None] * lp2
            for gp, lp1, lp2 in zip(gaussian_pyramid, laplacian_pyramid_1, laplacian_pyramid_2)
        ])
        result_mask += (result_mask ^ warped_mask)

    return np.clip(result_image, 0, 1)


def cylindrical_inverse_map(coords, h, w, scale):
    """Function that transform coordinates in the output image
    to their corresponding coordinates in the input image
    according to cylindrical transform.

    Use it in skimage.transform.warp as `inverse_map` argument

    coords ((M, 2) np.ndarray) : coordinates of output image (M == col * row)
    h (int) : height (number of rows) of input image
    w (int) : width (number of cols) of input image
    scale (int or float) : scaling parameter

    Returns:
        (M, 2) np.ndarray : corresponding coordinates of input image (M == col * row) according to cylindrical transform
    """
    # your code here
    if scale is None:
        scale = w / np.pi

    C = np.row_stack([coords[:, 0], coords[:, 1], np.ones(coords[:, 0].shape)])

    K = np.array([
        [scale, 0, w / 2],
        [0, scale, h / 2],
        [0, 0, 1]
    ])
    C_hat = inv(K) @ C

    B_hat = np.row_stack([
        np.tan(C_hat[0]),
        C_hat[1] / np.cos(C_hat[0]),
        np.ones(C_hat[0].shape)
    ])

    B = K @ B_hat
    return B[:2].T


def crop_image(image):
    img = rgb2gray(image)
    lr_nonzero = np.nonzero((img != 0).any(axis=0))[0]
    ud_nonzero = np.nonzero((img != 0).any(axis=1))[0]
    l, r = lr_nonzero[[0, -1]]
    u, d = ud_nonzero[[0, -1]]
    return image[u:d, l:r]


def warp_cylindrical(img, scale=None, crop=True):
    """Warp image to cylindrical coordinates

    img ((H, W, 3)  np.ndarray) : image for transformation
    scale (int or None) : scaling parameter. If None, defaults to W * 0.5
    crop (bool) : crop image to fit (remove unnecessary zero-padding of image)

    Returns:
        (H, W, 3)  np.ndarray : warped image (H and W may differ from original)
    """
    # your code here
    warped_image = warp(img, inverse_map=(
        lambda coordinates: cylindrical_inverse_map(
            coords=coordinates,
            h=img.shape[0],
            w=img.shape[1],
            scale=scale
        )
    ))
    if crop:
        return crop_image(warped_image)

    return warped_image


# Pick a good scale value for the 5 test image sets
cylindrical_scales = {
    0: 700,
    1: 700,
    2: 5000,
    3: 5000,
    4: 5000,
}
