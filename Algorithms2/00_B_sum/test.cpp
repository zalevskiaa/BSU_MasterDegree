#include "../test/test_runner.h"
#include "main.cpp"

#include <iostream>
#include <vector>

// ---------- SumMod ----------

class SumTestInfo {
public:
    struct Input {
        int first;
        int second;
    };

    struct Case {
        Input input;
        int output;
    };

    std::string name = "TestSum";
    std::vector<Case> cases_ = {
            {{1,  2},  3},
            {{11, 20}, 31},
    };

    int Run(const Input& input) const {
        return SumMod(input.first, input.second);
    }
};

// ---------- Solve ----------

class SolveTestInfo {
public:
    typedef std::string Input;
    typedef std::string Output;

    struct Case {
        std::string input;
        std::string output;
    };

    std::string name = "TestSolve";
    std::vector<Case> cases_ = {
            {"1 2",  "3"},
            {"11 20", "31"},
    };

    std::string Run(const std::string& input) const {
        std::stringstream is(input);
        std::stringstream os;
        Solve(is, os);
        return os.str();
    }
};

int main() {
    TestRunner tr;
    tr.RunTests(SumTestInfo());
    tr.RunTests(SolveTestInfo());

}
