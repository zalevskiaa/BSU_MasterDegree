// https://www.eolymp.com/ru/problems/1943

#include <iostream>

#include <vector>
#include <algorithm>

struct Edge {
    int to;
    int index;

    Edge(int to, int index) : to(to), index(index) {
    }
};

const int kMaxn = 50010;
std::vector<std::vector<Edge>> g(kMaxn);
std::vector<bool> used(kMaxn, false);
std::vector<int> tin(kMaxn);
std::vector<int> fup(kMaxn);

int timer;

std::vector<int> bridges;

void Dfs(int v, int p = -1) {
    used[v] = true;
    tin[v] = fup[v] = timer++;
    for (size_t i = 0; i < g[v].size(); ++i) {
        int to = g[v][i].to;
        if (to == p) {
            continue;
        }
        if (used[to]) {
            fup[v] = std::min(fup[v], tin[to]);
        } else {
            Dfs(to, v);
            fup[v] = std::min(fup[v], fup[to]);
            if (fup[to] > tin[v]) {
                // v-to - bridge
                bridges.push_back(g[v][i].index);
            }
        }
    }
}

void FindBridges(int n) {
    timer = 0;
    for (int i = 0; i < n; ++i) {
        used[i] = false;
    }
    for (int i = 0; i < n; ++i) {
        if (!used[i]) {
            Dfs(i);
        }
    }
}

int main() {
    int n, m;
    std::cin >> n >> m;
    for (int i = 0; i < m; ++i) {
        int a, b;
        std::cin >> a >> b;
        g[a].push_back({b, i + 1});
        g[b].push_back({a, i + 1});
    }

    FindBridges(n);

    std::sort(bridges.begin(), bridges.end());
    std::cout << bridges.size() << '\n';
    for (int i = 0; i < bridges.size(); ++i) {
        std::cout << bridges[i] << ' ';
    }
}