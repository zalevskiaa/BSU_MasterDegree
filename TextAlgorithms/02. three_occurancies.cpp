// Задача 6.2. Три вхождения
// Дана строка $s$, состоящая из строчных латинских букв.Необходимо найти максимальную по длине строку $w$, 
// которая встречается в $s$ хотя бы трижды без перекрытий.

// Формат входных данных
// Первая и единственная строка входного файла содержит одну строку $s$ ($3 \le |s| \le 10\,000$).
// Формат выходных данных
// Выведите одну строку — ответ на задачу. Если ответов несколько, выведите любой.Гарантируется, что существует такая непустая строка, которая встречается хотя бы три раза в строке $s$.
// Примеры
// input.txt               output.txt
// eetuppeetuppetupp       etupp
// bcacbacbaabacbbbacac    acb

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

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

struct DFSResult {
    int subtree_leaves_count = -1;
    std::string longest_prefix_with_3_leaves = "";
};

DFSResult Dfs(const SuffixTree& tree, int current=0, int current_depth=0, const std::string current_prefix="") {
    if (tree.nodes[current].to.empty()) {
        return {1, ""};
    }
    
    int subtree_leaves_count = 0;
    std::string longest_prefix_with_3_leaves = "";

    for (auto [edge, next] : tree.nodes[current].to) {
        int next_pos = tree.nodes[next].str_pos;
        int next_len = Min(static_cast<int>(tree.str_.size()) - tree.nodes[next].str_pos, tree.nodes[next].str_len);

        DFSResult dfs_result = Dfs(tree, next, current_depth + next_len, current_prefix + tree.str_.substr(next_pos, next_len));

        subtree_leaves_count += dfs_result.subtree_leaves_count;
        if (dfs_result.subtree_leaves_count >= 3 && dfs_result.longest_prefix_with_3_leaves.length() > longest_prefix_with_3_leaves.length()) {
            longest_prefix_with_3_leaves = dfs_result.longest_prefix_with_3_leaves;
        }
    }

    if (subtree_leaves_count >= 3 && current_prefix.length() > longest_prefix_with_3_leaves.length()) {
        longest_prefix_with_3_leaves = current_prefix;
    }

    return {subtree_leaves_count, longest_prefix_with_3_leaves};
}

std::string Solve(const std::string &str) {
    SuffixTree tree;
    tree.Build(str + '$');

    DFSResult dfs_result = Dfs(tree);

    std::string ans = dfs_result.longest_prefix_with_3_leaves;

    return ans;
}

//*

int main()
{
    std::ifstream fin("input.txt");
    std::ofstream fout("output.txt");

    std::string s;
    fin >> s;
    fout << Solve(s);

    return 0;
}

//*/