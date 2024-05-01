#include <iostream>
#include <string>
#include <vector>

std::vector<int> ZFunction(const std::string& s) {
    int size = static_cast<int>(s.length());
    std::vector<int> result(size);
    int l = 0, r = 0;
    for (int i = 1; i < size; ++i) {
        if (i <= r) {
            result[i] = std::min(result[i - l], r - i + 1);
        }
        while (i + result[i] < size && s[result[i]] == s[i + result[i]]) {
            ++result[i];
        }
        if (i + result[i] - 1 > r) {
            l = i, r = i + result[i] - 1;
        }
    }
    result[0] = size;
    return result;
}

void Solve(std::istream &is, std::ostream &os) {
    std::string str;
    is >> str;
    for (int elem: ZFunction(str)) {
        os << elem << ' ';
    }
    os << '\n';
}

//*

int main() {
    std::ios::sync_with_stdio(false);
    Solve(std::cin, std::cout);

    return 0;
}

//*/