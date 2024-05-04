#include <iostream>
#include <string>
#include <vector>

void PrefixFunction(const std::string& s, int left, std::vector<int>& result) {
    int size = static_cast<int>(s.length());
    result[left] = 0;
    for (int i = 1; i < size - left; ++i) {
        int j = result[left + i - 1];
        while (s[left + i] != s[left + j] && j > 0) {
            j = result[left + j - 1];
        }
        if (s[left + i] == s[left + j]) {
            ++j;
        }
        result[left + i] = j;
    }
}

int Solve(const std::string &sequence) {
    int result = 1;
    int size = static_cast<int>(sequence.length());
    std::vector<int> prefix_func(size);

    for (int left = 0; left < size - 1; ++left) {
        PrefixFunction(sequence, left, prefix_func);
        for (int right = left + 1; right < size; ++right) {
            int s_val = right - left + 1;
            if (s_val % (s_val - prefix_func[right]) == 0) {
                int new_result = s_val / (s_val - prefix_func[right]);
                if (new_result > result) {
                    result = new_result;
                }
            }
        }
    }

    return result;
}

//*

int main() {
    std::ios::sync_with_stdio(false);
    std::string sequence;
    std::cin >> sequence;
    std::cout << Solve(sequence);

    return 0;
}

//*/