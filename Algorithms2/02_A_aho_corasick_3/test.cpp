#include "../test/test_runner.h"
//#include "main_rioCFSI.cpp"
#include "fuzzy_matching.h"

#include <iostream>
#include <vector>

std::vector<size_t> SolveSlow(const std::string pattern, const std::string text) {
    std::vector<size_t> result;
    for (int pos = 0; pos < text.length() - pattern.length() + 1; ++pos) {
        bool match = true;
        for (int i = 0; i < pattern.length(); ++i) {
            if (pattern[i] != '?' && text[pos + i] != pattern[i]) {
                match = false;
                break;
            }
        }
        if (match) {
            result.push_back(pos);
        }
    }
    return result;
}

class SolveTestInfo {
public:
    struct Input {
        std::string pattern;
        std::string text;
    };
    struct Case {
        Input input;
        std::vector<size_t> output;
    };

    std::string name = "TestSolve";
    std::vector<Case> cases_ = {
            {"ab?", "ababcabc", {0, 2, 5}},
            {"???", "ababcabc", {0, 1, 2, 3, 4, 5}},
            {"a?", "aaa", {0, 1}},
            {"aa??", "aaaaa", {0, 1}},
            {"aa?????", "aaaaaaaa", {0, 1}},
            {"?a", "aaa", {0, 1}},
            {"??aa", "aaaaa", {0, 1}},
            {"?a?", "aaaaa", {0, 1, 2}},
            {"??aa??", "aaaaaaaa", {0, 1, 2}},
            {"aa?ab", "aaaabab", {0, 2}},
            {"aa?aa?", "aaaaaaaaaa", SolveSlow("aa?aa?", "aaaaaaaaaa")},
            {"abab?abab?", "aaaaaaaaaa", SolveSlow("abab?abab?", "aaaaaaaaaa")},
            {"abb?bb", "abbabbbbb", SolveSlow("abb?bb", "abbabbbbb")},
    };

    SolveTestInfo() {
        const int n = 1e5;
        std::vector<size_t> result;
        result.reserve(n);
        for (int i=0; i < 99980; ++i) {
            result.push_back(i);
        }
        std::string text = std::string(n, 'a');

        //return;

        cases_.push_back({"a?a?a?a?a?a?a?a?a?a?a", text, result});

        //return;

        std::string pattern;
        for (int i = 0; i < 10; ++i) {
            for (int j = 0; j < 1000; ++j) {
                pattern += 'a';
            }
            pattern += "?";
        }
        result.clear();
        for (int i=0; i < 89991; ++i) {
            result.push_back(i);
        }
        cases_.push_back({pattern, std::string(n, 'a'), result});

    }

    std::vector<size_t> Run(const Input &input) const {
        return FindFuzzyMatches(input.pattern, input.text, '?');
    }
};

int main() {
    TestRunner tr;
    tr.RunTests(SolveTestInfo());
}
