#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <string>
#include <map>

const int maxn = 100000;

struct PatternPos {
    std::string pattern;
    int pos;
};

std::vector<PatternPos> Tokenize(const std::string &pattern) {
    std::vector<PatternPos> patterns;
    for (int left = pattern.find_first_not_of('?'), right = pattern.find_first_of('?', left);
         left != std::string::npos;
         left = pattern.find_first_not_of('?', right),
                 right = pattern.find_first_of('?', left)) {
        patterns.push_back({pattern.substr(left, right - left), left});
    }
    return patterns;
}

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

        for (int i = 0; i < str.length(); ++i) {
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
        nodes_.reserve(maxn);
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
/*
std::vector<int> Solve(const std::string pattern, const std::string text) {
    std::vector<PatternPos> patterns = Tokenize(pattern);

    std::vector<int> result;
    result.reserve(100000);

    if (patterns.empty()) {
        for (int i = 0; i < text.length() - pattern.length() + 1; ++i) {
            result.push_back(i);
        }
        return result;
    }

    int end_question_marks = 0;
    for (int i = pattern.size() - 1; i >= 0; --i) {
        if (pattern[i] == '?') {
            end_question_marks += 1;
        } else {
            break;
        }
    }

    AhoCorasick ac(patterns);
    std::vector<std::vector<bool> > matches = ac.Match(text, patterns);

    for (int first_index = 0; first_index < matches[0].size(); ++first_index) {
        bool match = true;
        int index;

        for (int pattern_index = 0; pattern_index < matches.size(); ++pattern_index) {
            index = first_index + patterns[pattern_index].pos;
            if (index + patterns[pattern_index].pattern.length() + end_question_marks - 1 >= matches[0].size() ||
                !matches[pattern_index][index]) {
                match = false;
                break;
            }
        }

        if (match) {
            result.push_back(first_index);
        }
    }

    return result;
}
*/

//*

void Debug() {
    AhoCorasick ac({"a", "ab", "bc", "bca", "c", "caa"});

    int current = 0;
    for (char ch: std::string("ab$aba$abb$bca$bcaa$bcab$bcac$caa$caaa$caab$caac$")) {
        current = ac.Go(current, ch);
        std::cout << current << " : ";

        int link = current;
        while (link != 0) {
            std::cout << link << " ";
            link = ac.GetLink(link);
        }
        std::cout << " ; ";

        int out = current;
        while (out != 0) {
            std::cout << out << " ";
            out = ac.GetOut(out);
        }
        std::cout << "\n";
    }

    for (int i = 0; i < ac.nodes_.size(); ++i) {
        const Node &node = ac.nodes_[i];
        std::cout << i << " p " << node.parent << " p_ch " << node.parent_ch << " term " << node.terminate << " link "
                  << node.link << " out " << node.out;

        std::cout << " \tnext { ";
        for (const auto& pair : node.next) {
            std::cout << pair.first << " " << pair.second << ", ";
        }
        std::cout << "} \tgo { ";
        for (const auto& pair : node.go) {
            std::cout << pair.first << " " << pair.second << ", ";
        }
        std::cout << "}";

        std::cout << '\n';
    }

}

//*

int main() {
    //Debug();

    std::string pattern, text;
    //std::cin >> pattern >> text;
    pattern = "aa??";
    text = std::string(5, 'a');

    std::vector<int> result = Solve(pattern, text);

    std::cout << result.size() << '\n';
    for (int i = 0; i < result.size(); ++i) {
        std::cout << result[i] << ' ';
    }

    return 0;
}

//*/