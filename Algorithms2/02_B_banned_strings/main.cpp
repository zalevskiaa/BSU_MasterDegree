#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <map>

const int kMaxn = 100000;

struct Node {
    std::map<char, int> next;
    std::map<char, int> go;

    int parent;
    char parent_ch;
    bool terminate;
    int link;
    int out;

    Node(int par, char par_ch, bool term = false, int lnk = -1, int ot = -1)
            : parent(par), parent_ch(par_ch), terminate(term), link(lnk), out(ot) {}
};

class AhoCorasick {
public:
    std::vector<Node> nodes_;

    void AddString(const std::string str) {
        int current = 0;

        for (int i = 0; i < static_cast<int>(str.length()); ++i) {
            char ch = str[i];
            if (nodes_[current].next.find(ch) == nodes_[current].next.end()) {
                nodes_.push_back((Node(current, ch)));
                nodes_[current].next[ch] = nodes_.size() - 1;
            }

            current = nodes_[current].next[ch];
        }
        nodes_[current].terminate = true;
    }

public:
    AhoCorasick(const std::vector<std::string> &patterns) {
        nodes_.reserve(kMaxn);
        nodes_.push_back(Node(-1, 0, false));

        for (const std::string &str: patterns) {
            AddString(str);
        }
    }


    int GetOut(int current) {
        if (nodes_[current].out == -1) {

            if (current == 0 || nodes_[current].parent == 0) {
                nodes_[current].out = 0;

            } else {

                int link = GetLink(current);
                if (nodes_[link].terminate) {
                    nodes_[current].out = link;
                } else {
                    nodes_[current].out = GetOut(link);
                }

            }
        }
        return nodes_[current].out;
    }

    int GetLink(int current) {
        if (nodes_[current].link == -1) {
            if (current == 0 || nodes_[current].parent == 0) {
                nodes_[current].link = 0;
            } else {
                nodes_[current].link = Go(GetLink(nodes_[current].parent), nodes_[current].parent_ch);
            }
        }
        return nodes_[current].link;
    }

    int Go(int current, char ch) {
        if (nodes_[current].go.find(ch) == nodes_[current].go.end()) {
            if (nodes_[current].next.find(ch) == nodes_[current].next.end()) {

                if (current == 0) {
                    // current is root
                    nodes_[current].go[ch] = 0;
                } else {
                    nodes_[current].go[ch] = Go(GetLink(current), ch);
                }

            } else {
                nodes_[current].go[ch] = nodes_[current].next[ch];
            }
        }
        return nodes_[current].go[ch];
    }
};

int SumMod(int first, int second) {
    return (first + second) % 1'000'000'007;
}

int Solve(int text_length, const std::vector<std::string>& patterns, int vocabulary_size) {
    AhoCorasick aho_corasick(patterns);
    std::vector<std::vector<int>> dp(aho_corasick.nodes_.size(), std::vector<int>(text_length + 1));
    dp[0][0] = 1;
    for (int k = 0; k < text_length; ++k) {
        for (int current = 0; current < static_cast<int>(aho_corasick.nodes_.size()); ++current) {
            for (char ch = 'a'; ch < 'a' + vocabulary_size; ++ch) {
                int next = aho_corasick.Go(current, ch);
                if (aho_corasick.nodes_[next].terminate || aho_corasick.GetOut(next) != 0) {
                    continue;
                }
                dp[next][k + 1] = SumMod(dp[next][k + 1], dp[current][k]);
            }
        }
    }
    int result = 0;
    for (int current = 0; current < static_cast<int>(aho_corasick.nodes_.size()); ++current) {
        result = SumMod(result, dp[current][text_length]);
    }
    return result;
}

//*

int main() {
    int text_length;
    int patterns_number;
    int vocabulary_size;
    std::vector<std::string> patterns;

    std::cin >> text_length >> patterns_number >> vocabulary_size;

    patterns.reserve(patterns_number);
    for (int i = 0; i < patterns_number; ++i) {
        std::string str;
        std::cin >> str;
        patterns.push_back(str);
    }

    std::cout << Solve(text_length, patterns, vocabulary_size);

    return 0;
}

//*/