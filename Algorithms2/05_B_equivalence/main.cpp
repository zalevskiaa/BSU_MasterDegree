#include <iostream>
#include <vector>
#include <queue>

int n1, k1, l1;
int n2, k2, l2;
std::vector<std::vector<int>> automaton1;
std::vector<std::vector<int>> automaton2;
std::vector<bool> terminal1;
std::vector<bool> terminal2;
std::vector<std::vector<bool>> visited;

bool BFS() {
    std::queue<std::pair<int, int>> q;

    if (l1 != l2) {
        return false;
    }

    q.push({0, 0});
    while (!q.empty()) {
        std::pair<int, int> current = q.front();
        q.pop();

        visited[current.first][current.second] = true;

        if (terminal1[current.first] != terminal2[current.second]) {
            return false;
        }

        for (int i = 0; i < l1; i++) {
            int u = automaton1[current.first][i];
            int v = automaton2[current.second][i];
            if (!visited[u][v]) {
                q.push({u, v});
            }
        }
    }
    return true;
}

int main() {
    std::cin >> n1 >> k1 >> l1;

    terminal1.resize(n1, false);
    automaton1.resize(n1, std::vector<int>(l1, -1));

    for (int i = 0; i < k1; i++) {
        int x;

        std::cin >> x;
        terminal1[x] = true;
    }
    for (int i = 0; i < n1 * l1; i++) {
        int from, to;
        char ch;

        std::cin >> from >> ch >> to;
        automaton1[from][ch - 'a'] = to;
    }

    std::cin >> n2 >> k2 >> l2;

    terminal2.resize(n2, false);
    automaton2.resize(n2, std::vector<int>(l2, -1));

    for (int i = 0; i < k2; i++) {
        int x;

        std::cin >> x;
        terminal2[x] = true;
    }
    for (int i = 0; i < n2 * l2; i++) {
        int from, to;
        char ch;

        std::cin >> from >> ch >> to;
        automaton2[from][ch - 'a'] = to;
    }

    visited.resize(n1, std::vector<bool>(n2, false));

    if (BFS()) {
        std::cout << "EQUIVALENT";
    } else {
        std::cout << "NOT EQUIVALENT";
    }

    return 0;
}