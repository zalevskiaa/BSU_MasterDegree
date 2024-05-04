#include <iostream>

#include <vector>
#include <algorithm>

struct Edge {
    int to;
    int cost;

    Edge(int to, int cost) : to(to), cost(cost) {
    }
};

const int kMaxn = 50010;
std::vector<std::vector<Edge>> g(kMaxn);
std::vector<bool> used(kMaxn, false);
std::vector<int> tin(kMaxn);
std::vector<int> fup(kMaxn);

int timer;

bool bridge_found;
int min_cost;

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
                if (!bridge_found) {
                    min_cost = g[v][i].cost;
                    bridge_found = true;
                } else {
                    min_cost = std::min(min_cost, g[v][i].cost);
                }
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
        int a, b, c;
        std::cin >> a >> b >> c;
        g[a].push_back({b, c});
        g[b].push_back({a, c});
    }

    FindBridges(n);

    if (bridge_found) {
        std::cout << min_cost;
    } else {
        std::cout << -1;
    }
}