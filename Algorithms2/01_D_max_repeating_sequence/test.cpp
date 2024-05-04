#include "../test/test_runner.h"
#include "main.cpp"

#include <iostream>
#include <vector>

class SolveTestInfo {
public:
    typedef std::string Input;

    struct Case {
        std::string input;
        int output;
    };

    std::string name = "TestSolve";
    std::vector<Case> cases_ = {
            {"abaabaabaab", 3},
            {"abaabaabaabx", 3},
            {"xabaabaabaab", 3},
            {"xabaabaabaabx", 3},
            {"a", 1},
            {"ab", 1},
            {"aa", 2},
            {"aaa", 3},
            {"abc", 1},
            {"xabax", 1},
            {"xabcababcababcababcx", 3},
            {"aaaaaabbaaaaaabbaaaaaabbaaa", 6},
            {"aaabaabbaaabaabbaaabaabbaaa", 3},
    };

    SolveTestInfo() {
        const int n = 5e3;
        cases_.push_back({std::string(n, 'a'), n});
    }

    int Run(const std::string &input) const {
        return Solve(input);
    }
};

int main() {
    TestRunner tr;
    tr.RunTests(SolveTestInfo());
}
