#include <iostream>
#include <vector>

void BottomUpMerge(std::vector<int>& elems, size_t i_left, size_t i_right, size_t i_end,
                   std::vector<int>& buffer) {
    size_t index_left = i_left, index_right = i_right;
    for (size_t index = i_left; index < i_end; index++) {
        if (index_left < i_right &&
            (index_right >= i_end || elems[index_left] <= elems[index_right])) {
            buffer[index] = elems[index_left];
            index_left = index_left + 1;
        } else {
            buffer[index] = elems[index_right];
            index_right = index_right + 1;
        }
    }
}

void BottomUpMergeSort(std::vector<int>& elems) {
    std::vector<int> buffer(elems.size());
    for (size_t width = 1; width < elems.size(); width = 2 * width) {
        for (size_t index = 0; index < elems.size(); index = index + 2 * width) {
            BottomUpMerge(elems, index, std::min(index + width, elems.size()),
                          std::min(index + 2 * width, elems.size()), buffer);
        }
        std::move(buffer.begin(), buffer.end(), elems.begin());
    }
}

int main() {
    std::vector<int> a = {5, 4, 3, 2, 1, 11, 55, 44, 22, 33};

    for (size_t i = 0; i < a.size(); ++i) {
        std::cout << a[i] << ' ';
    }
    std::cout << std::endl;

    BottomUpMergeSort(a);

    for (size_t i = 0; i < a.size(); ++i) {
        std::cout << a[i] << ' ';
    }
    std::cout << std::endl;

    return 0;
}