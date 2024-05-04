#include "../test/test_runner.h"
#include "main.cpp"

#include <iostream>
#include <vector>

bool SolveSlow(int n, const std::vector<int>& current_wins, const std::vector<int>& future_matches_total,
               const std::vector<std::vector<int>>& future_matches_div) {
    return false;
}

class SolveTestInfo {
public:
    struct Input {
        int n;
        std::vector<int> current_wins;
        std::vector<int> future_matches_total;
        std::vector<std::vector<int>> future_matches_div;
    };
    struct Case {
        Input input;
        bool output;
    };

    std::string name = "TestSolve";
    std::vector<Case> cases_ = {
        {2, {2, 3}, {1, 1}, {{0, 1}, {1, 0}}, true},
        {2, {2, 3}, {1, 1}, {{0, 0}, {0, 0}}, true},
        {2, {2, 4}, {1, 1}, {{0, 1}, {1, 0}}, false},
        {2, {2, 4}, {1, 1}, {{0, 0}, {0, 0}}, false},
        {3, {2, 2, 1}, {0, 1, 1}, {{0, 0, 0}, {0, 0, 1}, {0, 1, 0}}, true},
        {3, {2, 1, 2}, {0, 1, 1}, {{0, 0, 0}, {0, 0, 1}, {0, 1, 0}}, true},
        {3, {1, 2, 2}, {1, 1, 1}, {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, true},
        {3, {1, 2, 2}, {1, 1, 1}, {{0, 0, 0}, {0, 0, 1}, {0, 1, 0}}, false},
        {2, {2, 3}, {0, 0}, {{0, 0}, {0, 0}}, false},
        {3, {2, 3, 3}, {0, 0, 0}, {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, false},
    };

    void GenerateSomeCases() {

    }

    SolveTestInfo() {
        GenerateSomeCases();
    }

    bool Run(const Input &input) const {
        return Solve(input.n, input.current_wins, input.future_matches_total, input.future_matches_div);
    }
};

int main() {
    TestRunner tr;
    tr.RunTests(SolveTestInfo());
}
