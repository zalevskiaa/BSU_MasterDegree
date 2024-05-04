class BinaryHeap {
private:
    std::vector<uint32_t> data_;

    void Heapify(uint32_t index) {
        uint32_t left_child, right_child, smallest_child;

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
    explicit BinaryHeap(uint32_t reserve_size = 0) {
        data_.reserve(reserve_size);
    }

    void BuildHeap(const std::vector<uint32_t>& elems)
    {
        data_ = elems;
        for (int i = data_.size() / 2; i >= 0; --i) {
            Heapify(i);
        }
    }

    void Push(uint32_t elem) {
        data_.push_back(elem);

        for (uint32_t index = data_.size() - 1, parent_index = (index - 1) / 2;
             index > 0 && data_[parent_index] > data_[index];
             index = parent_index, parent_index = (index - 1) / 2) {
            std::swap(data_[index], data_[parent_index]);
        }
    }
    uint32_t Pop() {
        uint32_t result = data_.front();
        std::swap(data_.front(), data_.back());
        data_.pop_back();
        Heapify(0);

        return result;
    }
    uint32_t Top() {
        return data_.front();
    }
    bool Empty() {
        return data_.empty();
    }
};
