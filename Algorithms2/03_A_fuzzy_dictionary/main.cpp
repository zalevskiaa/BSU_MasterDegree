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

void PrintTrie(const Trie& trie, int current = 0, const std::string& prefix = "") {
    std::cout << prefix << "    " << current << ' ' << trie.nodes_[current].terminate_id << '\n';
    for (auto& next_p : trie.nodes_[current].next) {
        PrintTrie(trie, next_p.second, "  " + prefix + "" + next_p.first);
    }
}

Trie MakeTrie0(const std::vector<std::string>& dictionary) {
    Trie trie_0;

    for (int i = 0; i < static_cast<int>(dictionary.size()); ++i) {
        const std::string& word = dictionary[i];

        int current = 0;
        for (char ch : word) {
            current = trie_0.Next(current, ch);
        }
        trie_0.nodes_[current].terminate_id = i;
    }
    return trie_0;
}

void BuildTrie1(Trie& trie_0, int current_0, Trie& trie_1, int current_1,
                bool wildcard_available = true) {
    if (trie_1.nodes_[current_1].terminate_id == -1) {
        trie_1.nodes_[current_1].terminate_id = trie_0.nodes_[current_0].terminate_id;
    }

    if (wildcard_available) {
        BuildTrie1(trie_0, current_0, trie_1, trie_1.Next(current_1, '?'), false);
    }
    for (auto& [ch_0, next_0] : trie_0.nodes_[current_0].next) {
        if (wildcard_available) {
            BuildTrie1(trie_0, next_0, trie_1, current_1, false);
            BuildTrie1(trie_0, next_0, trie_1, trie_1.Next(current_1, '?'), false);
        }
        BuildTrie1(trie_0, next_0, trie_1, trie_1.Next(current_1, ch_0), wildcard_available);
    }
}

Trie MakeTrie1(Trie& trie_0) {
    Trie trie_1;
    BuildTrie1(trie_0, 0, trie_1, 0);

    return trie_1;
}

int Check0(Trie& trie_0, const std::string& word) {
    int current = 0;
    for (char ch : word) {
        if (trie_0.nodes_[current].next.find(ch) == trie_0.nodes_[current].next.end()) {
            return -1;
        }
        current = trie_0.nodes_[current].next[ch];
    }
    return trie_0.nodes_[current].terminate_id;
}

int Check1(Trie& trie_1, int current_1, const std::string& word, int word_pos) {
    if (word_pos >= static_cast<int>(word.length())) {
        return trie_1.nodes_[current_1].terminate_id;
    }

    int result;

    char ch = word[word_pos];
    if (trie_1.nodes_[current_1].next.find(ch) != trie_1.nodes_[current_1].next.end()) {
        // dict ch == str ch
        result = Check1(trie_1, trie_1.nodes_[current_1].next[ch], word, word_pos + 1);
        if (result != -1) {
            return result;
        }
    }

    if (trie_1.nodes_[current_1].next.find('?') != trie_1.nodes_[current_1].next.end()) {
        // dict '?' == str any
        result = Check1(trie_1, trie_1.nodes_[current_1].next['?'], word, word_pos + 1);
        if (result != -1) {
            return result;
        }
    }

    return -1;
}

int Check2(Trie& trie_1, int current_1, const std::string& word, int word_pos) {
    if (word_pos >= static_cast<int>(word.length())) {
        return trie_1.nodes_[current_1].terminate_id;
    }

    int result;

    char ch = word[word_pos];
    if (trie_1.nodes_[current_1].next.find(ch) != trie_1.nodes_[current_1].next.end()) {
        // dict ch == str ch
        result = Check2(trie_1, trie_1.nodes_[current_1].next[ch], word, word_pos + 1);
        if (result != -1) {
            return result;
        }
    }
    if (trie_1.nodes_[current_1].next.find('?') != trie_1.nodes_[current_1].next.end()) {
        // dict '?' == str any
        result = Check2(trie_1, trie_1.nodes_[current_1].next['?'], word, word_pos + 1);
        if (result != -1) {
            return result;
        }
    }
    if (ch == '?') {
        // dict any == str '?'
        for (auto& [trie_ch, trie_next] : trie_1.nodes_[current_1].next) {
            result = Check2(trie_1, trie_next, word, word_pos + 1);
            if (result != -1) {
                return result;
            }
        }
    }

    return -1;
}

int Check2Word(Trie& trie_1, const std::string word) {
    int result;

    // replace
    std::string copy = word;

    for (int i = 0; i < static_cast<int>(copy.length()); ++i) {
        copy[i] = '?';
        result = Check2(trie_1, 0, copy, 0);
        // std::cout << copy << '\n';
        if (result != -1) {
            return result;
        }
        copy[i] = word[i];
    }

    // insert
    copy += '?';
    result = Check2(trie_1, 0, copy, 0);
    // std::cout << copy << '\n';
    if (result != -1) {
        return result;
    }
    for (int i = copy.length() - 2; i >= 0; --i) {
        std::swap(copy[i], copy[i + 1]);
        result = Check2(trie_1, 0, copy, 0);
        // std::cout << copy << '\n';
        if (result != -1) {
            return result;
        }
    }

    // remove
    copy = word.substr(1, word.length() - 1);
    result = Check2(trie_1, 0, copy, 0);
    // std::cout << copy << '\n';
    if (result != -1) {
        return result;
    }
    for (int i = 0; i < static_cast<int>(copy.length()); ++i) {
        copy[i] = word[i];
        result = Check2(trie_1, 0, copy, 0);
        // std::cout << copy << '\n';
        if (result != -1) {
            return result;
        }
    }
    return -1;
}

void Debug() {
    std::vector<std::string> dictionary = {"aaaa", "a"};
    Trie trie_0 = MakeTrie0(dictionary);
    PrintTrie(trie_0);

    Trie trie_1 = MakeTrie1(trie_0);
    PrintTrie(trie_1);

    for (const std::string& word : {"ac?"}) {
        std::cout << Check2(trie_0, 0, word, 0) << ' ';
    }

    for (const std::string& word : {"soap", "soup", "soa", "sou", "so", "s"}) {
        std::cout << Check0(trie_0, word) << ' ';
    }


    std::cout << '\n';
    for (const std::string& word : {"soap", "soup", "soa", "sou", "-oap", "s-ap", "so-p", "so-ap",
                                    "-soap", "soup-", "so", "s"}) {
        std::cout << Check1(trie_1, 0, word, 0) << ' ';
    }

    std::cout << '\n';
    for (const std::string& word : {"soap", "soup", "soa", "sou", "-oap", "s-ap", "so-p", "so-ap",
                                    "-soap", "soup-", "so", "ap", "up", "sp", "op"}) {
        std::cout << Check2Word(trie_1, word) << ' ';
    }

    std::cout << '\n';
    for (const std::string& word : {
             "-soa",
             "soap--",
             "--soap",
             "-soap-",
             "-so-up",
             "sou--p",
         }) {
        std::cout << Check2Word(trie_1, word) << ' ';
    }
}

std::vector<int> Solve(const std::vector<std::string>& dictionary,
                       const std::vector<std::string>& words) {
    std::vector<int> results;
    results.reserve(words.size());

    Trie trie_0 = MakeTrie0(dictionary);
    Trie trie_1 = MakeTrie1(trie_0);

    int result;
    for (const std::string& word : words) {
        result = Check0(trie_0, word);
        if (result != -1) {
            results.push_back(result);
            continue;
        }
        result = Check1(trie_1, 0, word, 0);
        if (result != -1) {
            results.push_back(result);
            continue;
        }
        result = Check2Word(trie_1, word);
        results.push_back(result);
    }
    return results;
}

//*

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int dict_size, words_size;
    std::vector<std::string> dictionary;
    dictionary.reserve(5010);
    std::vector<std::string> words;
    words.reserve(5010);

    std::string word;

    std::cin >> dict_size;
    for (int i = 0; i < dict_size; ++i) {
        std::cin >> word;
        dictionary.push_back(word);
    }
    std::cin >> words_size;
    for (int i = 0; i < words_size; ++i) {
        std::cin >> word;
        words.push_back(word);
    }

    for (int ans: Solve(dictionary, words)) {
        if (ans != -1) {
            std::cout << dictionary[ans] << '\n';
        } else {
            std::cout << "-1\n";
        }
    }

    std::cout << std::flush;

    return 0;
}

//*/
