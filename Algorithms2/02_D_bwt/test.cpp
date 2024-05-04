#include "../test/test_runner.h"
#include "main.cpp"

#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

std::string SolveSlow(const std::string& str) {
    int n = str.length();
    std::vector<std::string> matrix(n, std::string(n, '0'));

    matrix[0] = str;
    for (int i = 1; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            matrix[i][j] = matrix[i - 1][(j + 1) % n];
        }
    }
    std::sort(matrix.begin(), matrix.end());

    std::string result(n, '0');
    for (int i = 0; i < n; ++i) {
        result[i] = matrix[i][n - 1];
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
        std::string output;
    };

    std::string name = "TestSolve";
    std::vector<Case> cases_ = {
            {"ababc", "cbaab"},
            {"a", "a"},
            {"aaaaa", "aaaaa"},
            {"abcde", "eabcd"},
    };

    SolveTestInfo() {
        //return;

        for (const std::string str : {
                "a", "b", "aa", "bb", "ab", "aba", "aab", "abb",
                "abc", "abcab", "abcabc",
                "abcababc",
                "abbababb", "ababbabbbbaaabbababbbbabbb",
        }
                ) {
            cases_.push_back({str, SolveSlow(str)});
        }

        //return;

        AddSomeRandomCases(500, "slow");
        //AddSomeRandomCases(10000, "self");
    }

    std::string Run(const Input &input) const {
        return Solve(input.text);
    }

};

int main() {
    TestRunner tr;
    tr.RunTests(SolveTestInfo());
}
