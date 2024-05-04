#include <algorithm>
#include <cstring>
#include <deque>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>


template<class Iterator>
class IteratorRange {
public:
    IteratorRange(Iterator begin, Iterator end) : begin_(begin), end_(end) {}

    Iterator begin() const { return begin_; }

    Iterator end() const { return end_; }

private:
    Iterator begin_, end_;
};

namespace traverses {

// Traverses the connected component in a breadth-first order
// from the vertex 'origin_vertex'.
// Refer to
// https://goo.gl/0qYXzC
// for the visitor events.
    template<class Vertex, class Graph, class Visitor>
    void BreadthFirstSearch(Vertex origin_vertex, const Graph &graph,
                            Visitor visitor) {
        std::unordered_set<Vertex> visited;
        std::queue<Vertex> queue;

        queue.push(origin_vertex);
        visited.insert(origin_vertex);
        visitor.DiscoverVertex(origin_vertex);

        while (!queue.empty()) {
            Vertex current = queue.front();
            visitor.ExamineVertex(current);

            for (auto &next: OutgoingEdges(graph, current)) {
                visitor.ExamineEdge(next);

                if (visited.find(next) != visited.end()) {
                    continue;
                }
                queue.push(next);
                visited.insert(next);
                visitor.DiscoverVertex(next);
            }
            queue.pop();
        }
    }


// See "Visitor Event Points" on
// https://goo.gl/wtAl0y
    template<class Vertex, class Edge>
    class BfsVisitor {
    public:
        virtual void DiscoverVertex(Vertex /*vertex*/) {}

        virtual void ExamineEdge(const Edge & /*edge*/) {}

        virtual void ExamineVertex(Vertex /*vertex*/) {}

        virtual ~BfsVisitor() = default;
    };

} // namespace traverses

namespace aho_corasick {

    int debug_next_id = 1;

    struct AutomatonNode {
        AutomatonNode() : suffix_link(nullptr), terminal_link(nullptr) {}

        // Stores ids of strings which are ended at this node.
        std::vector<size_t> terminated_string_ids;
        // Stores tree structure of nodes.
        std::map<char, AutomatonNode> trie_transitions;

        // Stores cached transitions of the automaton, contains
        // only pointers to the elements of trie_transitions.
        std::map<char, AutomatonNode *> automaton_transitions_cache;
        AutomatonNode *suffix_link;
        AutomatonNode *terminal_link;
        int id;
    };

// Returns a corresponding trie transition 'nullptr' otherwise.
    AutomatonNode *GetTrieTransition(AutomatonNode *node, char character) {
        if (node->trie_transitions.find(character) == node->trie_transitions.end()) {
            return nullptr;
        }
        return &(node->trie_transitions[character]);
    }

// Go
// Returns an automaton transition, updates 'node->automaton_transitions_cache'
// if necessary.
// Provides constant amortized runtime.
    AutomatonNode *GetAutomatonTransition(AutomatonNode *node,
                                          const AutomatonNode *root,
                                          char character) {
        if (node->automaton_transitions_cache.find(character) == node->automaton_transitions_cache.end()) {
            // update
            if (GetTrieTransition(node, character) == nullptr) {
                if (node == root) {
                    node->automaton_transitions_cache[character] = node;
                } else {
                    node->automaton_transitions_cache[character] = GetAutomatonTransition(node->suffix_link, root,
                                                                                          character);
                }
            } else {
                node->automaton_transitions_cache[character] = &node->trie_transitions[character];
            }
        }
        return node->automaton_transitions_cache[character];
    }

    namespace internal {

        class AutomatonGraph {
        public:
            struct Edge {
                Edge(AutomatonNode *source, AutomatonNode *target, char character)
                        : source(source), target(target), character(character) {}

                AutomatonNode *source;
                AutomatonNode *target;
                char character;
            };
        };

        class SuffixLinkCalculator
                : public traverses::BfsVisitor<AutomatonNode *, AutomatonGraph::Edge> {
        public:
            explicit SuffixLinkCalculator(AutomatonNode *root) : root_(root) {}

            void ExamineVertex(AutomatonNode *node) override {
                if (node == root_) {
                    node->suffix_link = node;
                }
            }

            void ExamineEdge(const AutomatonGraph::Edge &edge) override {
                if (edge.source == root_) {
                    edge.target->suffix_link = root_;
                } else {
                    edge.target->suffix_link = GetAutomatonTransition(edge.source->suffix_link, root_, edge.character);
                }
            }

        private:
            AutomatonNode *root_;
        };

        class TerminalLinkCalculator
                : public traverses::BfsVisitor<AutomatonNode *, AutomatonGraph::Edge> {
        public:
            explicit TerminalLinkCalculator(AutomatonNode *root) : root_(root) {}

            void DiscoverVertex(AutomatonNode *node) override {
                if (node == root_) {
                    node->terminal_link = node;
                } else {
                    AutomatonNode *suffix = node->suffix_link;
                    if (suffix->terminated_string_ids.empty()) {
                        node->terminal_link = suffix->terminal_link;
                    } else {
                        node->terminal_link = suffix;
                    }
                }
                node->terminated_string_ids.insert(node->terminated_string_ids.end(),
                                                   node->terminal_link->terminated_string_ids.begin(),
                                                   node->terminal_link->terminated_string_ids.end());
            }

        private:
            AutomatonNode *root_;
        };

    } // namespace internal


    class NodeReference {
    public:
        NodeReference() : node_(nullptr), root_(nullptr) {}

        NodeReference(AutomatonNode *node, AutomatonNode *root)
                : node_(node), root_(root) {}

        NodeReference Next(char character) const {
            return NodeReference(GetAutomatonTransition(node_, root_, character), root_);
        }

        template<class Callback>
        void GenerateMatches(Callback on_match) const {
            for (size_t string_id: node_->terminated_string_ids) {
                on_match(string_id);
            }
        }

        IteratorRange<std::vector<size_t>::iterator> GetMatches() const {
            return IteratorRange<std::vector<size_t>::iterator>(node_->terminated_string_ids.begin(),
                                                                node_->terminated_string_ids.end());
        }

        explicit operator bool() const { return node_ != nullptr; }

        bool operator==(NodeReference other) const {
            return root_ == other.root_ && node_ == other.node_;
        }

    private:
        AutomatonNode *node_;
        AutomatonNode *root_;
    };

    class AutomatonBuilder;

    class Automaton {
    public:
        Automaton() = default;

        Automaton(const Automaton &) = delete;

        Automaton &operator=(const Automaton &) = delete;

        NodeReference Root() {
            return NodeReference(&root_, &root_);
        }

    private:
        AutomatonNode root_;

        friend class AutomatonBuilder;
    };

    class AutomatonBuilder {
    public:
        void Add(const std::string &string, size_t id) {
            words_.push_back(string);
            ids_.push_back(id);
        }

        std::unique_ptr<Automaton> Build() {
            auto automaton = std::make_unique<Automaton>();
            BuildTrie(words_, ids_, automaton.get());
            BuildSuffixLinks(automaton.get());
            BuildTerminalLinks(automaton.get());
            return automaton;
        }

    private:
        static void BuildTrie(const std::vector<std::string> &words,
                              const std::vector<size_t> &ids, Automaton *automaton) {
            for (size_t i = 0; i < words.size(); ++i) {
                AddString(&automaton->root_, ids[i], words[i]);
            }
        }

        static void AddString(AutomatonNode *root, size_t string_id,
                              const std::string &string) {
            AutomatonNode *current = root;

            for (int i = 0; i < static_cast<int>(string.length()); ++i) {
                char character = string[i];
                AutomatonNode *next = GetTrieTransition(current, character);

                if (next == nullptr) {
                    current->trie_transitions[character] = AutomatonNode();
                    next = &current->trie_transitions[character];
                    next->id = debug_next_id++;
                }

                current = next;
            }
            current->terminated_string_ids.push_back(string_id);
        }

        static std::vector<typename internal::AutomatonGraph::Edge> OutgoingEdges(
                const Automaton &graph, AutomatonNode *vertex) {
            std::vector<internal::AutomatonGraph::Edge> result;
            result.reserve(vertex->trie_transitions.size());
            for (std::map<char, AutomatonNode>::iterator iter = vertex->trie_transitions.begin();
                 iter != vertex->trie_transitions.end(); ++iter) {
                result.push_back(internal::AutomatonGraph::Edge(vertex, &iter->second, iter->first));
            }

            return result;
        }

        template<class Visitor>
        static void BreadthFirstSearch(AutomatonNode *origin_vertex, Automaton *graph,
                                       Visitor visitor) {
            std::unordered_set<AutomatonNode *> visited;
            std::queue<AutomatonNode *> queue;

            queue.push(origin_vertex);
            visited.insert(origin_vertex);
            visitor.DiscoverVertex(origin_vertex);

            while (!queue.empty()) {
                AutomatonNode *current = queue.front();
                visitor.ExamineVertex(current);

                for (internal::AutomatonGraph::Edge next: OutgoingEdges(*graph, current)) {
                    visitor.ExamineEdge(next);

                    if (visited.find(next.target) != visited.end()) {
                        continue;
                    }
                    queue.push(next.target);
                    visited.insert(next.target);
                    visitor.DiscoverVertex(next.target);
                }
                queue.pop();
            }
        }


        static void BuildSuffixLinks(Automaton *automaton) {
            BreadthFirstSearch(&automaton->root_, automaton,
                               internal::SuffixLinkCalculator(&automaton->root_));
        }

        static void BuildTerminalLinks(Automaton *automaton) {
            BreadthFirstSearch(&automaton->root_, automaton,
                               internal::TerminalLinkCalculator(&automaton->root_));
        }

        std::vector<std::string> words_;
        std::vector<size_t> ids_;
    };

} // namespace aho_corasick

// Consecutive delimiters are not grouped together and are deemed
// to delimit empty strings
template<class Predicate>
std::vector<std::string> Split(const std::string &string,
                               Predicate is_delimiter) {
    std::vector<std::string> result;

    int left = 0, right = 0;
    for (; right < static_cast<int>(string.length()); ++right) {
        if (is_delimiter(string[right])) {
            result.push_back(string.substr(left, right - left));
            left = right + 1;
        }
    }
    result.push_back(string.substr(left, right - left));
    return result;
}

// Wildcard is a character that may be substituted
// for any of all possible characters.
class WildcardMatcher {
public:
    WildcardMatcher() : number_of_words_(0), pattern_length_(0) {}

    WildcardMatcher static BuildFor(const std::string &pattern, char wildcard) {
        std::vector<std::string> words = Split(pattern, [wildcard](char character) { return character == wildcard; });

        WildcardMatcher result;
        result.number_of_words_ = 0;
        result.pattern_length_ = pattern.length();

        aho_corasick::AutomatonBuilder builder;
        size_t right = -1;
        for (const std::string &word: words) {
            right += word.length() + 1;
            if (word.length() > 0) {
                builder.Add(word, result.number_of_words_++);
                result.words_positions_in_pattern_.push_back(right - 1);
            }
        }

        result.aho_corasick_automaton_ = builder.Build();
        result.Reset();

        return result;
    }

    // Resets the matcher. Call allows to abandon all data which was already
    // scanned,
    // a new stream can be scanned afterwards.
    void Reset() {
        state_ = aho_corasick_automaton_->Root();
        current_pos_in_text_ = 0;

        words_occurrences_by_position_.clear();
        window_pos_in_text_ = 0;
    }

    template<class Callback>
    void Scan(char character, Callback on_match) {
        state_ = state_.Next(character);

        // shift-left : deque.back ~ current_pos_in_text_
        ShiftWordOccurrencesCountersRight();

        for (size_t id: state_.GetMatches()) {
            const size_t word_pos_in_pattern = words_positions_in_pattern_[id]; // word's last letter pos in pattern
            if (current_pos_in_text_ < word_pos_in_pattern) {
                continue;
            }
            size_t pattern_pos_in_text = current_pos_in_text_ - word_pos_in_pattern;
            words_occurrences_by_position_[pattern_pos_in_text - window_pos_in_text_] += 1;
        }

        if (words_occurrences_by_position_.size() == pattern_length_) {
            if (words_occurrences_by_position_.front() == number_of_words_) {
                on_match();
            }
            ShiftWordOccurrencesCountersLeft();
        }
        current_pos_in_text_ += 1;
    }

private:
    void ShiftWordOccurrencesCountersRight() {
        words_occurrences_by_position_.push_back(0);
    }

    void ShiftWordOccurrencesCountersLeft() {
        words_occurrences_by_position_.pop_front();
        window_pos_in_text_ += 1;
    }

    // Storing only O(|pattern|) elements allows us
    // to consume only O(|pattern|) memory for matcher.

    std::deque<size_t> words_occurrences_by_position_;
    std::vector<size_t> words_positions_in_pattern_;

    aho_corasick::NodeReference state_;
    size_t number_of_words_;
    size_t pattern_length_;
    size_t window_pos_in_text_;
    size_t current_pos_in_text_;
    std::unique_ptr<aho_corasick::Automaton> aho_corasick_automaton_;
};

std::string ReadString(std::istream &input_stream) {
    std::string str;
    input_stream >> str;
    return str;
}

// Return positions of the first character of an every match.
std::vector<size_t> FindFuzzyMatches(const std::string &pattern_with_wildcards,
                                     const std::string &text, char wildcard) {

    WildcardMatcher matcher = WildcardMatcher::BuildFor(pattern_with_wildcards, wildcard);

    std::vector<size_t> result;
    result.reserve(100000);
    for (int i = 0; i < static_cast<int>(text.length()); ++i) {
        matcher.Scan(text[i], [i, &pattern_with_wildcards, &result]() {
            result.push_back(i - pattern_with_wildcards.length() + 1);
        });
    }
    return result;
}

void Print(const std::vector<size_t> &sequence) {
    std::cout << sequence.size() << "\n";
    for (size_t elem: sequence) {
        std::cout << elem << ' ';
    }
    std::cout << '\n';
}