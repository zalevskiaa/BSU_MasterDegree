#include "../test/test_runner.h"
#include "main.cpp"

#include <iostream>
#include <vector>

std::vector<int> PrefixFunctionSlow(std::string s) {
    int n = static_cast<int>(s.length());
    std::vector<int> pi(n);
    for (int i = 0; i < n; ++i) {
        for (int k = 0; k <= i; ++k) {
            if (s.substr(0, k) == s.substr(i - k + 1, k)) {
                pi[i] = k;
            }
        }
    }
    return pi;
}

class PrefixFunctionTestInfo {
public:
    typedef std::string Input;

    struct Case {
        std::string input;
        std::vector<int> output;
    };

    std::string name = "TestPrefixFunction";
    std::vector<Case> cases_ = {
            {"abracadabra", {0, 0, 0, 1, 0, 1, 0, 1, 2, 3, 4}},
            {"aabaaab", {0, 1, 0, 1, 2, 2, 3}},
            {"abcabcd", {0, 0, 0, 1, 2, 3, 0}},
    };

    PrefixFunctionTestInfo() {
        const int max_n = 1e6;
        std::string input(max_n, 'a');
        std::vector<int> output(max_n);
        for (int i = 0; i < max_n; ++i) {
            output[i] = i;
        }
        cases_.push_back({input, output});
    }

    std::vector<int> Run(const Input &input) const {
        return PrefixFunction(input);
    }
};

int main() {
    TestRunner tr;
    tr.RunTests(PrefixFunctionTestInfo());
}
