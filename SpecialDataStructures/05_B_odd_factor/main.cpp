#include <iostream>
#include <vector>
#include <stack>
#include <unordered_set>
#include <cassert>
#include <fstream>

std::unordered_set<int> InitializeUnused(int n) {
    std::unordered_set<int> unused;
    for (int i = 0; i < n; ++i) {
        unused.insert(i);
    }
    return unused;
}

bool TryUse(std::unordered_set<int> *unused, int vertex) {
    std::unordered_set<int>::const_iterator iter = unused->find(vertex);
    if (iter == unused->end()) {
        return false;
    }
    unused->erase(iter);
    return true;
}

struct Edge {
    int from;
    int to;
};

bool FindOddFactorComponent(const std::vector<std::vector<int>> &list_graph, std::vector<Edge> *result,
                            std::unordered_set<int> *unused,
                            std::vector<int> *degrees) {
    std::stack<Edge> stack;

    int root = *unused->begin();

    stack.push({-1, root});
    unused->erase(root);

    while (true) {
        int previous = stack.top().from;
        int current = stack.top().to;

        bool found = false;
        for (int next: list_graph[current]) {
            if (TryUse(unused, next)) {
                stack.push({current, next});
                found = true;
            }
        }
        if (found) {
            continue;
        }

        bool current_good = degrees->at(current) & 1;

        stack.pop();
        if (stack.empty()) {
            return current_good;
        }
        if (current_good) {
            continue;
        }

        result->push_back({previous, current});
        ++degrees->at(previous);

        if (stack.empty()) {
            return false;
        }
    }

    return false;
}

bool FindOddFactor(const std::vector<std::vector<int>> &list_graph, std::vector<Edge> *result) {
    if (list_graph.size() & 1) {
        return false; // todo: not sure about it, try remove if fail
    }

    std::unordered_set<int> unused(InitializeUnused(list_graph.size()));
    std::vector<int> degrees(list_graph.size());
    while (true) {
        if (!FindOddFactorComponent(list_graph, result, &unused, &degrees)) {
            return false;
        }
        if (unused.empty()) {
            break;
        }
    }

    return true;
}

bool CheckIsOddFactor(int n, const std::vector<Edge> &edges_list) {
    std::vector<int> degrees(n, 0);
    for (const auto&[f, s]: edges_list) {
        assert(f < degrees.size());
        assert(s < degrees.size());
        degrees[f] += 1;
        degrees[s] += 1;
    }
    for (int d: degrees) {
        if (d % 2 == 0) {
            return false;
        }
    }
    return true;
}

bool CheckOddFactorExists(int n, const std::vector<Edge> &edges_list) {
    std::vector<bool> edges_mask(edges_list.size() + 1);

    while (!edges_mask[edges_mask.size() - 1]) {
        std::vector<Edge> edges_subset;
        for (int i = 0; i < edges_list.size(); ++i) {
            if (edges_mask[i]) {
                edges_subset.push_back(edges_list[i]);
            }
        }
        if (CheckIsOddFactor(n, edges_subset)) {
            return true;
        }

        int i = 0;
        while (edges_mask[i]) {
            edges_mask[i] = false;
            ++i;
        }
        edges_mask[i] = true;
    }

    return false;
}

std::vector<std::vector<int>> SqueezedGraphToListGraph(int n, const std::vector<bool> &squeezed_graph) {
    std::vector<std::vector<int>> result(n);

    for (int i = 0, i_sg = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j, ++i_sg) {
            if (squeezed_graph[i_sg]) {
                result[i].push_back(j);
                result[j].push_back(i);
            }
        }
    }

    return result;
}

void Test() {
    int n = 6;

    int total = 0;
    int exist = 0;
    int exist_correct = 0;
    int not_exist = 0;
    int not_exist_correct = 0;

    std::vector<bool> squeezed_graph(n * (n - 1) / 2 + 1);

    while (!squeezed_graph[squeezed_graph.size() - 1]) {
        std::vector<std::vector<int>> list_graph = SqueezedGraphToListGraph(n, squeezed_graph);
        std::vector<Edge> edges_list;

        ++total;
        if (FindOddFactor(list_graph, &edges_list)) {
            ++exist;
            if (CheckIsOddFactor(n, edges_list)) {
                ++exist_correct;
            }
        } else {
            ++not_exist;
            if (!CheckOddFactorExists(n, edges_list)) {
                ++not_exist_correct;
            }
        }

        int i = 0;
        while (squeezed_graph[i]) {
            squeezed_graph[i] = false;
            ++i;
        }
        squeezed_graph[i] = true;
    }

    std::cout << '\n';
    std::cout << "total: " << total << '\n';
    std::cout << "exist:         " << exist << '\n';
    std::cout << "exist_correct: " << exist_correct << '\n';
    std::cout << "not_exist:         " << not_exist << '\n';
    std::cout << "not_exist_correct: " << not_exist_correct << '\n';
}

void Solve() {
    std::ifstream fin("oddfactor.in");
    std::ofstream fout("oddfactor.out");

    int n, m;
    fin >> n >> m;

    std::vector<std::vector<int>> list_graph(n);
    for (int i = 0; i < m; ++i) {
        int a, b;
        fin >> a >> b;
        --a;
        --b;
        list_graph[a].push_back(b);
        list_graph[b].push_back(a);
    }

    std::vector<Edge> edges_list;
    edges_list.reserve(n);
    if (FindOddFactor(list_graph, &edges_list)) {
        fout << edges_list.size() << '\n';
        for (const auto& [a, b]: edges_list) {
            fout << a + 1 << " " << b + 1 << '\n';
        }
    } else {
        fout << -1;
    }
}

int main() {
//    Test();
    Solve();
}