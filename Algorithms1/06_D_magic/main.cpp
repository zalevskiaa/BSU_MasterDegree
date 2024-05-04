#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

const int kMaxN = 100002, kLgMaxN = 20, kMaxM = 500002;

struct PList {
    int value, num_edge;
    PList* next;
};

int n_val, m_val, root, k_val;
int component_count, global_time;

std::vector<bool> bridge(kMaxM + 1);
std::vector<bool> used(kMaxM + 1);

std::vector<int> component(kMaxN + 1);
std::vector<int> length(kMaxN + 1);
std::vector<int> time_in(kMaxN + 1);
std::vector<int> max_time_in(kMaxN + 1);
std::vector<int> min_time_in(kMaxN + 1);
std::vector<std::vector<int>> parents(kMaxN + 1, std::vector<int>(kLgMaxN + 1));

PList *tree_edges[kMaxN + 1], *graph_edges[kMaxN + 1];

void DFS(int current, int parent, bool pass_bridges) {
    PList* temp;
    bool pass_bridge;

    ++global_time;
    time_in[current] = global_time;
    min_time_in[current] = global_time;
    used[current] = true;
    temp = graph_edges[current];
    component[current] = component_count;

    while (temp != nullptr) {
        if (pass_bridges && bridge[temp->num_edge]) {
            pass_bridge = false;
        } else {
            pass_bridge = true;
        }
        if (!used[temp->value] && pass_bridge) {
            DFS(temp->value, current, pass_bridges);
            if (!pass_bridges) {
                if (min_time_in[temp->value] > time_in[current]) {
                    bridge[temp->num_edge] = true;
                }
                min_time_in[current] = std::min(min_time_in[current], min_time_in[temp->value]);
            }
        } else if (pass_bridge && temp->value != parent && time_in[temp->value] < min_time_in[current]) {
            min_time_in[current] = time_in[temp->value];
        }
        temp = temp->next;
    }
}

void AddTreeEdge(int a, int b) {
    PList* x = new PList;
    x->next = tree_edges[a];
    x->value = b;
    tree_edges[a] = x;
}

void DFSTree(int num, int parent, int cur_length) {
    PList* temp;
    temp = tree_edges[num];
    used[num] = true;

    length[num] = cur_length;
    parents[num][0] = parent;
    ++global_time;
    time_in[num] = global_time;
    max_time_in[num] = global_time;
    while (temp != nullptr) {
        if (!used[temp->value]) {
            DFSTree(temp->value, num, cur_length + 1);
            if (max_time_in[temp->value] > max_time_in[num]) {
                max_time_in[num] = max_time_in[temp->value];
            }
        }
        temp = temp->next;
    }
}

void Preprocess() {
    PList* temp;

    DFS(root, root, false);
    for (int i = 1; i <= kMaxN; ++i) {
        used[i] = false;
        component[i] = 0;
    }
    component_count = 0;
    for (int i = 1; i <= n_val; ++i) {
        if (!used[i]) {
            ++component_count;
            DFS(i, i, true);
        }
    }
    for (int i = 1; i <= n_val; ++i) {
        temp = graph_edges[i];
        while (temp != nullptr) {
            if (bridge[temp->num_edge]) {
                AddTreeEdge(component[temp->value], component[i]);
                AddTreeEdge(component[i], component[temp->value]);
            }
            temp = temp->next;
        }
    }
    // checked to here
    for (int i = 1; i <= kMaxN; ++i) {
        time_in[i] = 0;
        max_time_in[i] = 0;
        max_time_in[i] = 0;
        for (int j = 0; j <= kLgMaxN; ++j) {
            parents[i][j] = 0;
        }
        used[i] = 0;
        length[i] = 0;
    }
    global_time = 0;
    DFSTree(component[root], component[root], 0);
    for (int j = 1; j <= kLgMaxN; ++j) {
        for (int i = 1; i <= component_count; ++i) {
            if (used[i]) {
                parents[i][j] = parents[parents[i][j - 1]][j - 1];
            }
        }
    }
}

void AddEdge(int a, int b, int c) {
    PList* x = new PList;
    x->value = b;
    x->next = graph_edges[a];
    x->num_edge = c;
    graph_edges[a] = x;
}

bool IsParent(int first, int second) {
    if (time_in[second] >= time_in[first] && time_in[second] <= max_time_in[first]) {
        return true;
    } else {
        return false;
    }
}

int LCA(int first, int second) {
    if (IsParent(first, second)) {
        return first;
    } else if (IsParent(second, first)) {
        return second;
    } else {
        for (int i = kLgMaxN; i >= 0; --i) {
            if (!IsParent(parents[first][i], second)) {
                first = parents[first][i];
            }
        }
        return parents[first][0];
    }
}

int Solve(int first, int second) {
    int index = LCA(first, second);
    return length[index];
}

int main() {
    int from, to;
    for (int i = 1; i <= kMaxM; ++i) {
        bridge[i] = false;
    }
    std::cin >> n_val >> m_val >> root;

    for (int i = 1; i <= m_val; ++i) {
        std::cin >> from >> to;
        AddEdge(from, to, i);
        AddEdge(to, from, i);
    }
    Preprocess();
    std::cin >> k_val;
    for (int i = 1; i <= k_val; ++i) {
        std::cin >> from >> to;
        std::cout << Solve(component[from], component[to]) << '\n';
    }
}