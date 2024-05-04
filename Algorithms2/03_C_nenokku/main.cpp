#include <iostream>
#include <string>
#include <vector>
#include <map>

struct Node {
    std::map<char, int> to;
    int str_pos, str_len;
    int link;

    Node() : str_pos(0), str_len(0), link(0) {
    }

    Node(int pos, int len) : str_pos(pos), str_len(len), link(0) {
    }
};

class SuffixTree {
public:
    const int inf;
    const int max_n;
    std::string str_;

    std::vector<Node> nodes;

    int current_node = 0, current_pos = 0;

public:
    SuffixTree() : inf(1e9), max_n(100500) {
        for (int i = 0; i < static_cast<int>(nodes.size()); ++i) {
            nodes[i].to.clear();
        }
        nodes.clear();
        nodes.reserve(3 * max_n);
        nodes.push_back(Node(0, inf));

        current_node = 0;
        current_pos = 0;

        str_.reserve(max_n);
    }

    int MakeNode(int pos, int len) {
        nodes.push_back(Node(pos, len));
        return nodes.size() - 1;
    }

    void GoEdge(int index) {
        while (current_pos > nodes[nodes[current_node].to[str_[index - current_pos + 1]]].str_len) {
            current_node = nodes[current_node].to[str_[index - current_pos + 1]];
            current_pos -= nodes[current_node].str_len;
        }
    }

    void AddLetter(int index) {
        char ch = str_[index];

        current_pos++;

        int last = 0;
        while (current_pos > 0) {

            GoEdge(index);
            int edge = str_[index - current_pos + 1];

            int tmp = str_[nodes[nodes[current_node].to[edge]].str_pos + current_pos - 1];

            if (nodes[current_node].to[edge] == 0) {
                nodes[current_node].to[edge] = MakeNode(index - current_pos + 1, inf);

                nodes[last].link = current_node;
                last = 0;

            } else if (tmp == ch) {
                nodes[last].link = current_node;
                return;

            } else {

                int u = MakeNode(nodes[nodes[current_node].to[edge]].str_pos, current_pos - 1);
                nodes[u].to[ch] = MakeNode(index, inf);
                nodes[u].to[tmp] = nodes[current_node].to[edge];
                nodes[nodes[current_node].to[edge]].str_pos += current_pos - 1;
                nodes[nodes[current_node].to[edge]].str_len -= current_pos - 1;

                nodes[current_node].to[edge] = u;

                nodes[last].link = u;
                last = u;
            }

            if (current_node == 0) {
                current_pos -= 1;
            } else {
                current_node = nodes[current_node].link;
            }
        }
    }

    void Build(const std::string& str) {
        str_ += str;
        for (int i = static_cast<int>(str_.size()) - static_cast<int>(str.size()); i < static_cast<int>(str_.size()); i++) {
            AddLetter(i);
        }
    }
};

void LowerCase(std::string& str) {
    for (char& chr: str) {
        chr = std::tolower(chr);
    }
}

int Min(int first, int second) {
    return first < second ? first : second;
}
int EdgeLen(const SuffixTree& tree, int next) {
    return Min(static_cast<int>(tree.str_.size()) - tree.nodes[next].str_pos, tree.nodes[next].str_len);
}

bool Find(SuffixTree& tree, const std::string& pattern) {
    int current = 0;
    int pattern_index = 0;

    while (pattern_index < static_cast<int>(pattern.length())) {
        char ch_next = pattern[pattern_index];
        if (tree.nodes[current].to.find(ch_next) == tree.nodes[current].to.end()) {
            // trie transition not exist
            return false;
        }

        int next = tree.nodes[current].to[ch_next];

        int next_pos = tree.nodes[next].str_pos;
        int next_len = EdgeLen(tree, next);

        if (next_len > static_cast<int>(pattern.length()) - pattern_index) {
            // end of pattern at the edge
            for (int i = 0; i < static_cast<int>(pattern.length()) - pattern_index; ++i) {
                if (pattern[pattern_index + i] != tree.str_[next_pos + i]) {
                    return false;
                }
            }
            return true;
        }

        // move over the edge
        for (int i = 0; i < next_len; ++i) {
            if (pattern[pattern_index + i] != tree.str_[next_pos + i]) {
                return false;
            }
        }
        pattern_index += next_len;
        current = next;
    }

    return true;
}

void Solve() {
    SuffixTree tree;

    char type;
    std::string text;

    int counter = 0;

    while (std::cin >> type >> text) {

        LowerCase(text);

        if (type == '?') {

            bool result = Find(tree, text);

            if (result) {
                std::cout << "YES\n";
            } else {
                std::cout << "NO\n";
            }

            ++counter;
            if (counter > 100000) {
                std::cout << std::flush;
                counter = 0;
            }

        } else {

            tree.Build(text);
        }
    }
    std::cout << std::flush;

}

//*

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    Solve();

    return 0;
}

//*/