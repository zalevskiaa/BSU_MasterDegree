#include <iostream>

#include <vector>
#include <algorithm>

int Contains(const std::vector<int>& sequence, int elem_to_find) {
    for (int elem : sequence) {
        if (elem == elem_to_find) {
            return true;
        }
    }
    return false;
}

int UniqueElemsNumber(const std::vector<int>& sequence) {
    std::vector<int> unique_elems;
    unique_elems.reserve(sequence.size());
    for (int elem : sequence) {
        if (!Contains(unique_elems, elem)) {
            unique_elems.push_back(elem);
        }
    }
    return unique_elems.size();
}

int UniqueTreesNumber(int size) {
    const int module = 123456789;

    std::vector<int> result(size + 1, 0);
    result[0] = 1;
    result[1] = 1;

    for (int subsize = 2; subsize < size + 1; ++subsize) {
        for (int root = 1; root < subsize + 1; ++root) {
            result[subsize] =
                (result[subsize] +
                 (static_cast<int64_t>(result[root - 1]) * result[subsize - root]) % module) %
                module;
        }
    }

    return result[size];
}

int UniqueTreesNumber(std::vector<int> sequence) {
    std::vector<std::vector<int>> result(sequence.size() + 1,
                                         std::vector<int>(sequence.size() + 1, 1));
    for (size_t len = 1; len <= sequence.size(); ++len) {
        for (size_t pos = 0; pos <= sequence.size() - len; ++pos) {
            size_t left = pos, right = pos + len;
            int64_t cnt = 0;
            for (size_t root = left; root < right; ++root) {
                if (root > left && sequence[root] == sequence[root - 1]) {
                    continue;
                }
                cnt +=
                    static_cast<int64_t>(result[left][root]) * result[root + 1][right] % 123456789;
                cnt %= 123456789;
            }
            result[left][right] = cnt;
        }
    }
    return result[0][sequence.size()];
}

int main() {
    int size;
    std::cin >> size;
    std::vector<int> sequence(size);
    for (int index = 0; index < size; ++index) {
        std::cin >> sequence[index];
        // sequence[index] = index;
    }

    std::sort(sequence.begin(), sequence.end());

    // std::cout << UniqueTreesNumber(UniqueElemsNumber(sequence));
    // std::cout << '\n_val';
    std::cout << UniqueTreesNumber(sequence);
}