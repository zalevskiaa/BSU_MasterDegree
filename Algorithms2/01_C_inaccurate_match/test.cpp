#include "../test/test_runner.h"
#include "main.cpp"

#include <iostream>
#include <vector>

class SolveTestInfo {
public:
    typedef std::string Input;

    struct Case {
        std::string input;
        std::string output;
    };

    std::string name = "TestSolve";
    std::vector<Case> cases_ = {
            {"aaaa\nCaaabdaaaa", "4\n1 2 6 7 "},
            {"aaab\nCaaabdaaaa", "2\n2 7 "},
            {"aba\nbababbaabba", "5\n2 4 5 8 9 "},
            {"a\na", "1\n1 "},
            {"aa\naba", "2\n1 2 "},
            {"a\nb", "1\n1 "},
            {"aaaaaa\naaa", "0\n"},
            {"aaaaaa\naaaa", "0\n"},
            {"aaaaaa\naaaaa", "0\n"},
            {"aa\na", "0\n"},
            {"aaa\na", "0\n"},

    };

    SolveTestInfo() {
        const int n = 1e6, l = 1e6;
        std::string text(n, 'a');
        std::string pattern(l, 'a');

        int k = n - l + 1;
        std::stringstream output;
        output << k << '\n';
        for (int i = 0; i < k; ++i) {
            output << i + 1 << ' ';
        }
        cases_.push_back({pattern + '\n' + text, output.str()});
    }

    std::string Run(const std::string& input) const {
        std::stringstream is(input);
        std::stringstream os;
        Solve(is, os);
        return os.str();
    }
};

int main() {
    TestRunner tr;
    tr.RunTests(SolveTestInfo());
}
