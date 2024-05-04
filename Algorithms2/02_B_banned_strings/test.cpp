#include "../test/test_runner.h"
#include "main.cpp"

#include <iostream>
#include <vector>

class SolveTestInfo {
public:
    struct Input {
        std::string pattern;
        std::string text;
    };
    struct Case {
        Input input;
        std::vector<int> output;
    };

    std::string name = "TestSolve";
    std::vector<Case> cases_ = {

    };

    SolveTestInfo() {

    }

    std::vector<int> Run(const Input &input) const {
        return Solve(input.pattern, input.text);
    }
};

int main() {
    TestRunner tr;
    tr.RunTests(SolveTestInfo());
}
