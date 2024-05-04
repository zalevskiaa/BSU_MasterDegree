#include "../test/test_runner.h"
#include "main.cpp"

#include <iostream>
#include <vector>

template<typename T>
typename T::size_type LevenshteinDistance(const T &source,
                                          const T &target) {
    if (source.size() > target.size()) {
        return LevenshteinDistance(target, source);
    }

    using TSizeType = typename T::size_type;
    const TSizeType min_size = source.size(), max_size = target.size();
    std::vector<TSizeType> lev_dist(min_size + 1);

    for (TSizeType i = 0; i <= min_size; ++i) {
        lev_dist[i] = i;
    }

    for (TSizeType j = 1; j <= max_size; ++j) {
        TSizeType previous_diagonal = lev_dist[0], previous_diagonal_save;
        ++lev_dist[0];

        for (TSizeType i = 1; i <= min_size; ++i) {
            previous_diagonal_save = lev_dist[i];
            if (source[i - 1] == target[j - 1]) {
                lev_dist[i] = previous_diagonal;
            } else {
                lev_dist[i] = std::min(std::min(lev_dist[i - 1], lev_dist[i]), previous_diagonal) + 1;
            }
            previous_diagonal = previous_diagonal_save;
        }
    }

    return lev_dist[min_size];
}

std::vector<int> SolveSlow(std::vector<std::string>& dictionary, std::vector<std::string>& words) {
    std::vector<int> result(words.size(), -1);
    std::vector<int> dist(words.size(), -1);

    for (int i = 0; i < words.size(); ++i) {
        for (int j = 0; j < dictionary.size(); ++j) {
            int distance = LevenshteinDistance(words[i], dictionary[j]);
            if (distance > 2) {
                continue;
            }
            if (dist[i] == -1 || distance < dist[i]) {
                dist[i] = distance;
                result[i] = j;
            }
        }
    }
    return result;
}
std::vector<int> SolveSlowDistances(std::vector<std::string>& dictionary, std::vector<std::string>& words) {
    std::vector<int> result(words.size(), -1);
    std::vector<int> dist(words.size(), -1);

    for (int i = 0; i < words.size(); ++i) {
        for (int j = 0; j < dictionary.size(); ++j) {
            int distance = LevenshteinDistance(words[i], dictionary[j]);
            if (distance > 2) {
                continue;
            }
            if (dist[i] == -1 || distance < dist[i]) {
                dist[i] = distance;
                result[i] = j;
            }
        }
    }
    return dist;
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
        {{{"aaaa", "a"}, {"aa"}}, {}},
        {{{"aac", "abc", "bac", "bbc", "cac", "ac"}, {"bbc"}},{3}},
        {{{"hello", "world", "test"}, {"helo", "wodrl", "testt", "world", "tsef"}}, {0, 1, 2, 1, -1}},
        {{{"soap", "soup"}, {"soap", "soup"}}, {0, 1}},
        {{{"soap"}, {"oap", "sap", "sop", "soa", "-soap", "s-oap", "so-ap", "soa-p", "soap-", "-oap", "s-ap", "so-p", "soa-"}}, {}},
        {{{"soap"}, {"ap", "sp", "so", "so", "-sop", "s--oap", "so-a-p", "so-a-p", "s-oap-", "-oa-", "s-ap", "s--p", "soa--"}}, {}},
        {{{"soap"}, {"a", "s", "o", "xso", "x-sop", "s--oaxp", "so---p", "so-a-p", "s-oap-", "-oa-", "s-ap", "s--p", "soa--"}}, {}},
        {{{"a"}, {"a", "aa", "aaa", "ab", "ba", "bb", "b", "abb", "bab"}}, {}},
        {{{"abcd"}, {"a", "b", "c", "d", "acb"}}, {-1, -1, -1, -1, 0}},
        {{{""}, {"", "b", "c", "d", "ac"}}, {0, 0, 0, 0, 0}},
        {{{"a"}, {""}}, {0}},
        {{{"ab"}, {""}}, {0}},
    };

    void GenerateSomeCases() {
        std::vector<std::string> words;
        std::vector<std::string> d1, d2, d3, d4, d5, d6, d7, d8, d9, d10;

        std::string letters = "abcd";
        char max_ch = 'e';
        for (letters[0] = 'a'; letters[0] < max_ch; ++letters[0]) {
            for (letters[1] = 'a'; letters[1] < max_ch; ++letters[1]) {
                for (letters[2] = 'a'; letters[2] < max_ch; ++letters[2]) {
                    for (letters[3] = 'a'; letters[3] < max_ch; ++letters[3]) {
                        for (letters[4] = 'a'; letters[4] < max_ch; ++letters[4]) {
                            for (letters[5] = 'a'; letters[5] < max_ch; ++letters[5]) {

                                std::string word;
                                for (char letter: letters) {
                                    if (letter < max_ch - 1) {
                                        word += letter;
                                    }
                                }
                                if (word.length() < 1) {
                                    continue;
                                }
                                words.push_back(word);

                                uint64_t val = (letters[0] - 'a') + 2 * (letters[1] - 'a') + 3 * (letters[2] - 'a') + 5 * (letters[3] - 'a') + 7 * (letters[4] - 'a') + 11 * (letters[5] - 'a');
                                if (val % 20 < 2) {
                                    d1.push_back(word);
                                } else if (val % 20 < 4) {
                                    d2.push_back(word);
                                } else if (val % 20 < 6) {
                                    d3.push_back(word);
                                } else if (val % 20 < 8) {
                                    d4.push_back(word);
                                } else if (val % 20 < 10) {
                                    d5.push_back(word);
                                } else if (val % 20 < 12) {
                                    d6.push_back(word);
                                } else if (val % 20 < 14) {
                                    d7.push_back(word);
                                } else if (val % 20 < 16) {
                                    d8.push_back(word);
                                } else if (val % 20 < 18) {
                                    d9.push_back(word);
                                } else if (val % 20 < 20) {
                                    d10.push_back(word);
                                }
                            }
                        }
                    }
                }
            }
        }

        std::vector<std::vector<std::string>> dicts = {
            {"aaaa", "bbbb", "cccc"},
            {"aaabbb", "bbbbaaa"},
            {"abcdef", "bcdef", "abcd", "defab", "aabbccd"},
            {"abccef", "abcccf", "abdddf", "bcefdef", "axxdef", "aaaaa", "aabbbbb"},
            {"", "a", "aa", "aaa", "aaaa", "ab", "ba", "aba", "bab", "abaa"},
            d1, d2, d3, d4, d5, d6, d7, d8, d9, d10,
        };
        for (const std::vector<std::string>& dict : dicts) {
            cases_.push_back({{dict, words}, {}});
        }
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

    bool CheckAnswerCorrect(const Input &input, const std::vector<int>& actual, const std::vector<int>& expected) {
        for (int i = 0; i < input.words.size(); ++i) {
            if (actual[i] == expected[i]) {
                continue;
            }
            if (actual[i] == -1 || expected[i] == -1) {
                return false;
            }
            const std::string& word = input.words[i];
            const std::string& word_d_actual = input.dictionary[actual[i]];
            const std::string& word_d_expected = input.dictionary[expected[i]];

            int actual_distance = LevenshteinDistance(word, word_d_actual);
            int expected_distance = LevenshteinDistance(word, word_d_expected);
            if (actual_distance != expected_distance) {
                std::cout << "    \n";
                std::cout << "    dict " << input.dictionary << '\n';
                std::cout << "    index " << i << " word " << word << '\n';
                std::cout << "    actual " << word_d_actual << " " << actual_distance << '\n';
                std::cout << "    expected " << word_d_expected << " " << expected_distance << '\n';
                return false;
            }
        }
        return true;
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
        std::cout << (sti.CheckAnswerCorrect(sti.cases_[ci].input, actual, sti.cases_[ci].output) ? "true" : "false") << "\n";

        continue;
        if (ci == 6) {
            std::cout << sti.cases_[ci].input.dictionary << '\n';
            std::cout << sti.cases_[ci].input.words << '\n';
            std::cout << actual << '\n';
            std::cout << sti.cases_[ci].output << "\n\n";
        }
    }
}
