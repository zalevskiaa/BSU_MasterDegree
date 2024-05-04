#include "../test/test_runner.h"
#include "main.cpp"

#include <iostream>
#include <vector>

std::vector<int> SolveSlow(std::vector<std::string>& dictionary, std::vector<std::string>& words) {

}

class SolveTestInfo {
public:
    struct Input {
        std::vector<std::string> dictionary;
        std::vector<std::string> words;
    };
    struct Case {
        Input input;
        std::vector<int> output;
    };

    std::string name = "TestSolve";
    std::vector<Case> cases_ = {

    };

    void GenerateSomeCases() {

    }

    SolveTestInfo() {
        GenerateSomeCases();

        for (auto& test_case: cases_) {
            if (test_case.output.empty()) {
                test_case.output = SolveSlow(test_case.input.dictionary, test_case.input.words);
            }
        }
    }

    std::vector<int> Run(const Input &input) const {
        return Solve(input.dictionary, input.words);
    }
};

int main() {
    //TestRunner tr;
    //tr.RunTests(SolveTestInfo());
    SolveTestInfo sti;
    for (int ci = 0; ci < sti.cases_.size(); ++ci) {
        std::cout << "case " << ci << " ";
        std:: cout << sti.cases_[ci].input.dictionary.size() << " ";
        std::vector<int> actual = sti.Run(sti.cases_[ci].input);
        std::cout << ((actual == sti.cases_[ci].output) ? "true" : "false") << "\n";

        continue;
        if (ci == 6) {
            std::cout << sti.cases_[ci].input.dictionary << '\n';
            std::cout << sti.cases_[ci].input.words << '\n';
            std::cout << actual << '\n';
            std::cout << sti.cases_[ci].output << "\n\n";
        }
    }
}
