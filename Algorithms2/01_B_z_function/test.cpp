#include "../test/test_runner.h"
#include "main.cpp"

#include <iostream>
#include <vector>

class ZFunctionTestInfo {
public:
    typedef std::string Input;

    struct Case {
        std::string input;
        std::vector<int> output;
    };

    std::string name = "TestZFunction";
    std::vector<Case> cases_ = {
            {"abracadabra", {11, 0, 0, 1, 0, 1, 0, 4, 0, 0, 1}},
            {"aabaaab", {7, 1, 0, 2, 3, 1, 0}},
            {"abcabcd", {7, 0, 0, 3, 0, 0, 0}},
    };

    ZFunctionTestInfo() {
        const int max_n = 1e7;
        std::string input(max_n, 'a');
        std::vector<int> output(max_n);
        for (int i = 0; i < max_n; ++i) {
            output[i] = max_n - i;
        }
        cases_.push_back({input, output});
    }

    std::vector<int> Run(const Input &input) const {
        return ZFunction(input);
    }
};

int main() {
    TestRunner tr;
    tr.RunTests(ZFunctionTestInfo());
}
