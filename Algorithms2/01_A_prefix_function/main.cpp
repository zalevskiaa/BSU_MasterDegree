#include <iostream>
#include <string>
#include <vector>

std::vector<int> PrefixFunction(const std::string& s) {
    int size = static_cast<int>(s.length());
    std::vector<int> result(size);
    for (int i = 1; i < size; ++i) {
        int j = result[i - 1];
        while (s[i] != s[j] && j > 0) {
            j = result[j - 1];
        }
        if (s[i] == s[j]) {
            ++j;
        }
        result[i] = j;
    }
    return result;
}

void Solve(std::istream &is, std::ostream &os) {
    std::string str;
    is >> str;
    for (int elem: PrefixFunction(str)) {
        os << elem << ' ';
    }
}

//*

int main() {
    std::ios::sync_with_stdio(false);
    Solve(std::cin, std::cout);

    return 0;
}

//*/