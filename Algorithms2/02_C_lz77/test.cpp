#include "../test/test_runner.h"
#include "main.cpp"

#include <iostream>
#include <vector>
#include <set>

std::vector<int> SolveSlow(const std::string& str) {
    std::vector<int> result(str.length());

    for (int i = 0; i < str.length(); ++i) {
        for (int j = 0; j < i; ++j) {
            int l = 0;
            while (str[i + l] == str[j + l]) {
                l += 1;
            }
            if (l > result[i]) {
                result[i] = l;
            }
        }
    }

    return result;
}

class SolveTestInfo {
private:
    void AddSomeRandomCases(int n, std::string method) {
        std::vector<std::string> strings;

        std::string str(n, 'a');
        strings.push_back(str);

        for (int i = 0; i < n; ++i) {
            if (i % 3 == 0) {
                str[i] = 'b';
            }
        }
        strings.push_back(str);

        for (int i = 0; i < n; ++i) {
            if (i % 7 < 4) {
                str[i] = 'b';
            }
        }
        strings.push_back(str);

        for (int i = 0; i < n; ++i) {
            if (i % 17 < 4) {
                str[i] = 'b';
            }
        }
        strings.push_back(str);

        for (int i = 0; i < n; ++i) {
            if (i % 19 == 0) {
                str[i] = 'b';
            }
        }
        strings.push_back(str);

        for (int i = 0; i < n; ++i) {
            if (i % 39 < 3) {
                str[i] = 'b';
            }
        }
        strings.push_back(str);

        for (int i = 0; i < n; ++i) {
            if (i % 59 < 17) {
                str[i] = 'b';
            }
        }
        strings.push_back(str);

        for (int i = 0; i < n; ++i) {
            if (i % 7 < 2) {
                str[i] = 'c';
            }
        }
        strings.push_back(str);

        for (int i = 0; i < n; ++i) {
            if (i % 17 < 2) {
                str[i] = 'c';
            }
        }
        strings.push_back(str);

        for (int i = 0; i < n; ++i) {
            if (i % 19 == 1) {
                str[i] = 'c';
            }
        }
        strings.push_back(str);

        for (int i = 0; i < n; ++i) {
            if (i % 39 < 2) {
                str[i] = 'c';
            }
        }
        strings.push_back(str);

        for (int i = 0; i < n; ++i) {
            if (i % 59 < 7) {
                str[i] = 'c';
            }
        }
        strings.push_back(str);

        for (int i = 0; i < strings.size(); ++i) {
            if (method == "slow") {
                cases_.push_back({strings[i], SolveSlow(strings[i])});
            } else if (method == "self") {
                cases_.push_back({strings[i], Solve(strings[i])});
            } else if (method == "empty") {
                cases_.push_back({strings[i], {}});
            } else {
                std::cout << "fuck";
                return;
            }
        }
    }

public:
    struct Input {
        std::string text;
    };
    struct Case {
        Input input;
        std::vector<int> output;
    };

    std::string name = "TestSolve";
    std::vector<Case> cases_ = {
            {"ababaab", {0, 0, 3, 2, 1, 2, 1}},
            {"aaaaa", {0, 4, 3, 2, 1}},
    };

    SolveTestInfo() {
        for (const std::string str : {
                "", "a", "b", "aa", "bb", "ab", "aba", "aab", "abb",
                "abbababb", "ababbabbbbaaabbababbbbabbb",}
                ) {
            cases_.push_back({str, SolveSlow(str)});
        }

        AddSomeRandomCases(500, "slow");
        AddSomeRandomCases(10000, "self");
    }

    std::vector<int> Run(const Input &input) const {
        return Solve(input.text);
    }

};

int main() {
    TestRunner tr;
    tr.RunTests(SolveTestInfo());
}
