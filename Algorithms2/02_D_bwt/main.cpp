#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <stack>

struct Node {
    std::map<int, int> to;
    int str_pos, str_len;
    int link;

    Node() : str_pos(0), str_len(0), link(0) {}

    Node(int pos, int len) : str_pos(pos), str_len(len), link(0) {}
};

class SuffixTree {
public:

    const int inf;
    const int max_n;
    std::string str_;

    std::vector<Node> nodes;

    int current_node = 0, current_pos = 0;

public:
    SuffixTree() : inf(1e9), max_n(100000) {
        for (int i = 0; i < static_cast<int>(nodes.size()); ++i) {
            nodes[i].to.clear();
        }
        nodes.clear();
        nodes.reserve(3 * max_n);
        nodes.push_back(Node(0, inf));

        current_node = 0;
        current_pos = 0;
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
            //int v = nodes[current_node].to[edge];
            int tmp = str_[nodes[nodes[current_node].to[edge]].str_pos + current_pos - 1];

            if (nodes[current_node].to[edge] == 0) {
                nodes[current_node].to[edge] = MakeNode(index - current_pos + 1, inf);
                //nodes[current_node].to[edge] = v;

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
                //nodes[current_node].to[edge] = v;

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

    void Build(const std::string &str) {
        str_ = str;
        for (int i = 0; i < static_cast<int>(str_.size()); i++) {
            AddLetter(i);
        }
    }
};

int Min(int first, int second) {
    return first < second ? first : second;
}

std::vector<int> depths;

void Dfs(const SuffixTree &tree, int current, int current_depth) {
    if (tree.nodes[current].to.empty()) {
        depths.push_back(current_depth);
    }
    for (std::pair<int, int> p_next: tree.nodes[current].to) {
        int next = p_next.second;

        int next_len = Min(static_cast<int>(tree.str_.size()) - tree.nodes[next].str_pos, tree.nodes[next].str_len);
        Dfs(tree, next, current_depth + next_len);
    }
}

std::string Solve(const std::string &str) {
    SuffixTree tree;
    tree.Build(str + '$');

    depths.clear();
    depths.reserve(100010);
    Dfs(tree, 0, 0);

    std::string result;
    result.reserve(str.length());
    for (int i = 0; i < static_cast<int>(depths.size()); ++i) {
        int index = static_cast<int>(str.length()) - depths[i] + 1;

        if (index == 0) {
            continue;
        }

        result += str[(index + str.length() - 1) % str.length()];
    }
    return result;
}

/*

int main() {
    std::string s;
    s = "racaa";
    //std::cin >> s;

    std::string result = Solve(s);

    std::cout << result;

    return 0;
}

//*/