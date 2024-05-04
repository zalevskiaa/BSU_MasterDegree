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

std::vector<int> depths(3 * 100000, 0);
std::vector<int> results(3 * 100000, 0);

void Dfs(const SuffixTree& tree, int current, int current_depth) {
    depths[current] = current_depth;

    for (std::pair<int, int> p_next : tree.nodes[current].to) {
        int next = p_next.second;

        int next_len = Min(static_cast<int>(tree.str_.size()) - tree.nodes[next].str_pos, tree.nodes[next].str_len);
        Dfs(tree, next, current_depth + next_len);
    }
}

int Dfs2(const SuffixTree& tree, int current) {
    //std::cout << "current " << current << '\n';

    std::vector<int> nexts;
    int lowest_next_index = -1;

    for (std::pair<int, int> p_next : tree.nodes[current].to) {
        int next = p_next.second;

        if (tree.nodes[next].to.empty()) {
            // next is leaf
            nexts.push_back(next);

        } else {
            // next is not leaf
            int min_next = Dfs2(tree, next);
            nexts.push_back(min_next);
        }

        if (lowest_next_index == -1 || depths[nexts.back()] > depths[nexts[lowest_next_index]]) {
            lowest_next_index = nexts.size() - 1;
        }
    }

    for (int i = 0; i < static_cast<int>(nexts.size()); ++i) {
        if (i != lowest_next_index) {
            results[tree.str_.length() - depths[nexts[i]]] = depths[current];
        }
    }

    return nexts[lowest_next_index];
}

std::vector<int> Solve(const std::string &str) {
    std::fill(depths.begin(), depths.end(), 0);
    std::fill(results.begin(), results.end(), 0);

    SuffixTree tree;
    tree.Build(str + '&');

    Dfs(tree, 0, 0);
    Dfs2(tree, 0);

    std::vector<int> result(str.length(), 0);
    for (int i = 0; i < static_cast<int>(result.size()); ++i) {
        result[i] = results[i];
    }
    return result;
}

//*

int main()
{
    std::string s;
    //s = "abbababb";
    std::cin >> s;

    std::vector<int> result = Solve(s);

    for (int i = 0; i < static_cast<int>(result.size()); ++i) {
        std::cout << result[i] << '\n';
    }

    return 0;
}

//*/