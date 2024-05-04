#include <iostream>
#include <vector>
#include <algorithm>

struct HeapElem {
    int value;
    size_t seq_index;

    HeapElem(int val, size_t ind) {
        value = val;
        seq_index = ind;
    }

    bool operator>(const HeapElem& other) const {
        return value > other.value;
    }
};

class BinaryHeap {
private:
    std::vector<HeapElem> data_;

    void Heapify(size_t index) {
        size_t left_child, right_child, smallest_child;

        smallest_child = index;
        left_child = 2 * index + 1;
        right_child = 2 * index + 2;

        if (left_child < data_.size() && data_[smallest_child] > data_[left_child]) {
            smallest_child = left_child;
        }
        if (right_child < data_.size() && data_[smallest_child] > data_[right_child]) {
            smallest_child = right_child;
        }

        while (smallest_child != index) {
            std::swap(data_[index], data_[smallest_child]);
            index = smallest_child;

            smallest_child = index;
            left_child = 2 * index + 1;
            right_child = 2 * index + 2;

            if (left_child < data_.size() && data_[smallest_child] > data_[left_child]) {
                smallest_child = left_child;
            }
            if (right_child < data_.size() && data_[smallest_child] > data_[right_child]) {
                smallest_child = right_child;
            }
        }
    }

public:
    explicit BinaryHeap(size_t reserve_size = 0) {
        data_.reserve(reserve_size);
    }

    void Push(HeapElem elem) {
        data_.push_back(elem);

        for (size_t index = data_.size() - 1, parent_index = (index - 1) / 2;
             index > 0 && data_[parent_index] > data_[index];
             index = parent_index, parent_index = (index - 1) / 2) {
            std::swap(data_[index], data_[parent_index]);
        }
    }
    HeapElem Pop() {
        HeapElem result = data_.front();
        std::swap(data_.front(), data_.back());
        data_.pop_back();
        Heapify(0);

        return result;
    }
    bool Empty() {
        return data_.empty();
    }
};

std::vector<int> Merge(const size_t n_val, const size_t m_val,
                       const std::vector<std::vector<int>> sequences) {
    BinaryHeap heap(n_val * m_val);
    std::vector<size_t> indices(n_val, 0);

    for (size_t seq_index = 0; seq_index < indices.size(); ++seq_index) {
        heap.Push(HeapElem(sequences[seq_index][indices[seq_index]++], seq_index));
    }

    std::vector<int> result;
    result.reserve(n_val * m_val);

    while (!heap.Empty()) {
        HeapElem elem = heap.Pop();
        result.push_back(elem.value);

        if (indices[elem.seq_index] < sequences[elem.seq_index].size()) {
            heap.Push(
                HeapElem(sequences[elem.seq_index][indices[elem.seq_index]++], elem.seq_index));
        }
    }

    return result;
}

int main() {
    size_t n_val, m_val;
    std::cin >> n_val >> m_val;
    std::vector<std::vector<int>> sequences(n_val, std::vector<int>(m_val));
    for (size_t i = 0; i < n_val; ++i) {
        for (size_t j = 0; j < m_val; ++j) {
            std::cin >> sequences[i][j];
        }
    }

    std::vector<int> result(Merge(n_val, m_val, sequences));

    for (size_t i = 0; i < result.size(); ++i) {
        std::cout << result[i] << ' ';
    }
}
