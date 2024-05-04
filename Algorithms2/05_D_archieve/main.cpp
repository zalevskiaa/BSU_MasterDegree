#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <map>

struct Node {
    std::map<char, int> next;

    // bool terminate;
    int terminate_id;

    Node() : /*terminate(term),*/ terminate_id(-1) {
    }
};

class Trie {
public:
    std::vector<Node> nodes_;

    Trie() {
        nodes_.push_back(Node());
    }

    int Next(int current, char ch) {
        if (nodes_[current].next.find(ch) == nodes_[current].next.end()) {
            nodes_[current].next[ch] = nodes_.size();
            nodes_.push_back(Node());
        }
        return nodes_[current].next[ch];
    }
};

int main() {


    return 0;
}