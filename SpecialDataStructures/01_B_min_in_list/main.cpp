#include <iostream>
#include <stack>
#include <string>

struct StackElem {
    int value;
    int min_value;
};

class MinStack : public std::stack<StackElem> {
public:
    void Push(int value) {
        if (empty()) {
            push({value, value});
        } else {
            push({value, std::min(value, top().min_value)});
        }
    }
};

class List {
public:
    void PushFront(int value) {
        left_.Push(value);
    }
    void PushBack(int value) {
        right_.Push(value);
    }
    void PopFront() {
        if (left_.empty()) {
            std::stack<int> buffer;
            int buffer_size = right_.size() / 2;
            for (int i = 0; i < buffer_size; ++i) {
                buffer.push(right_.top().value);
                right_.pop();
            }

            while (!right_.empty()) {
                left_.Push(right_.top().value);
                right_.pop();
            }
            while (!buffer.empty()) {
                right_.Push(buffer.top());
                buffer.pop();
            }
        }
        left_.pop();
    }
    void PopBack() {
        if (right_.empty()) {
            std::stack<int> buffer;
            int buffer_size = left_.size() / 2;
            for (int i = 0; i < buffer_size; ++i) {
                buffer.push(left_.top().value);
                left_.pop();
            }
            while (!left_.empty()) {
                right_.Push(left_.top().value);
                left_.pop();
            }
            while (!buffer.empty()) {
                left_.Push(buffer.top());
                buffer.pop();
            }
        }
        right_.pop();
    }
    int Min() const {
        if (left_.empty()) {
            if (right_.empty()) {
                return -1;
            } else {
                return right_.top().min_value;
            }
        } else if (right_.empty()) {
            return left_.top().min_value;
        } else {
            return std::min(left_.top().min_value, right_.top().min_value);
        }
    }

private:
    MinStack left_;
    MinStack right_;
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    List list;

    std::string cmd;
    int value;
    for (int i = 0; i < n; ++i) {
        std::cin >> cmd;
        if (cmd[0] == '+') {
            std::cin >> value;
            if (cmd[1] == 'L') {
                list.PushFront(value);
            } else {
                list.PushBack(value);
            }
        } else {
            if (cmd[1] == 'L') {
                list.PopFront();
            } else {
                list.PopBack();
            }
        }
        std::cout << list.Min() << '\n';
    }

    std::cout << std::flush;
    return 0;
}
