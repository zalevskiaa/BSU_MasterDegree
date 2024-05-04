#include "../test/test_runner.h"
#include "main.cpp"

#include <iostream>
#include <vector>
#include <set>

int64_t SolveSlow(const std::string& str) {
    std::set<std::string> unique;
    for (int pos = 0; pos < str.length(); ++pos) {
        for (int len = 1; len < str.length() - pos + 1; ++len) {
            unique.insert(str.substr(pos, len));
        }
    }
    return unique.size();
}

class SolveTestInfo {
public:
    struct Input {
        std::string text;
    };
    struct Case {
        Input input;
        int64_t output;
    };

    std::string name = "TestSolve";
    std::vector<Case> cases_ = {
            {"abc", 6},
            {"aba", 5},
            {"aaa", 3},
    };

    SolveTestInfo() {
        for (const std::string str : {
            "", "a", "b", "aa", "bb", "ab", "aba", "aab", "abb",
            "abbababb", "ababbabbbbaaabbababbbbabbb",}
            ) {
            cases_.push_back({str, SolveSlow(str)});
        }

        int n = 1000;
        std::string str(n, 'a');
        cases_.push_back({str, SolveSlow(str)});

        for (int i = 0; i < n; ++i) {
            if (i % 3 == 0) {
                str[i] = 'b';
            }
        }
        cases_.push_back({str, SolveSlow(str)});

        for (int i = 0; i < n; ++i) {
            if (i % 7 < 4) {
                str[i] = 'b';
            }
        }
        cases_.push_back({str, SolveSlow(str)});

        for (int i = 0; i < n; ++i) {
            if (i % 17 < 4) {
                str[i] = 'b';
            }
        }
        cases_.push_back({str, SolveSlow(str)});

        for (int i = 0; i < n; ++i) {
            if (i % 19 == 0) {
                str[i] = 'b';
            }
        }
        cases_.push_back({str, SolveSlow(str)});

        for (int i = 0; i < n; ++i) {
            if (i % 39 < 3) {
                str[i] = 'b';
            }
        }
        cases_.push_back({str, SolveSlow(str)});

        for (int i = 0; i < n; ++i) {
            if (i % 59 < 17) {
                str[i] = 'b';
            }
        }
        cases_.push_back({str, SolveSlow(str)});

        for (int i = 0; i < n; ++i) {
            if (i % 7 < 2) {
                str[i] = 'c';
            }
        }
        cases_.push_back({str, SolveSlow(str)});

        for (int i = 0; i < n; ++i) {
            if (i % 17 < 2) {
                str[i] = 'c';
            }
        }
        cases_.push_back({str, SolveSlow(str)});

        for (int i = 0; i < n; ++i) {
            if (i % 19 == 1) {
                str[i] = 'c';
            }
        }
        cases_.push_back({str, SolveSlow(str)});

        for (int i = 0; i < n; ++i) {
            if (i % 39 < 2) {
                str[i] = 'c';
            }
        }
        cases_.push_back({str, SolveSlow(str)});

        for (int i = 0; i < n; ++i) {
            if (i % 59 < 7) {
                str[i] = 'c';
            }
        }
        cases_.push_back({str, SolveSlow(str)});
    }

    int64_t Run(const Input &input) const {
        return Solve(input.text);
    }
};

int main() {
    TestRunner tr;
    tr.RunTests(SolveTestInfo());
}
