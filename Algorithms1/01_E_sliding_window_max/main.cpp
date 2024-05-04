#include <iostream>

#include <deque>
#include <vector>

struct Item {
    int value;
    size_t index;
};

class SlidingWindow {
private:
    size_t front_index_, back_index_;
    std::deque<Item> max_items_;

public:
    SlidingWindow() {
        front_index_ = 0;
        back_index_ = 0;
    }
    void PushBack(int value) {
        while (!max_items_.empty() && max_items_.back().value <= value) {
            max_items_.pop_back();
        }
        max_items_.push_back({ value, back_index_++ });
    }
    void PopFront() {
        if (max_items_.front().index == front_index_) {
            max_items_.pop_front();
        }
        front_index_++;
    }
    int MaxValue() const {
        if (max_items_.empty()) {
            // this is a wtf case, according to the task condition
            return 0;
        }
        return max_items_.front().value;
    }
};

int main() {
    size_t sequence_size;
    std::cin >> sequence_size;

    std::vector<int> sequence;
    sequence.reserve(sequence_size);

    for (size_t i = 0; i < sequence_size; ++i) {
        std::cin >> sequence[i];
    }

    SlidingWindow sliding_window;
    size_t sequence_index = 0;
    sliding_window.PushBack(sequence[sequence_index++]);

    size_t queries_count;
    std::cin >> queries_count;
    for (size_t q = 0; q < queries_count; ++q) {
        char query;
        std::cin >> query;

        if (query == 'R') {
            sliding_window.PushBack(sequence[sequence_index++]);
            std::cout << sliding_window.MaxValue() << ' ';
        } else {
            sliding_window.PopFront();
            std::cout << sliding_window.MaxValue() << ' ';
        }
    }

    return 0;
}
