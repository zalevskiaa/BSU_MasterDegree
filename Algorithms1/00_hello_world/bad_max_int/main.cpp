#include <iostream>
#include <vector>

const int BAD_MAX_INT = (1 << 31) - 1;

int main() {
    size_t n;
    std::cin >> n;
    std::vector<int> data(n);
    for (int i = 0; i < data.size(); ++i) {
        std::cin >> data[i];
    }
    return 0;
}