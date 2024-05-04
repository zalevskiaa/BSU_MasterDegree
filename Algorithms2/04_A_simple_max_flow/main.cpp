#include <vector>
#include <iostream>
#include <list>
#include <queue>
#include <cassert>

struct Node {
    std::vector<int> next;
    bool visited;

    Node() : visited(false) {
    }
};

class Graph {
public:
    std::vector<Node> nodes_;

public:
    Graph(int number_of_nodes) {
        nodes_ = std::vector<Node>(number_of_nodes);
    }

    void AddEdge(int u, int v) {
        nodes_[u].next.push_back(v);
        nodes_[v].next.push_back(u);
    }
};

int Bfs(Graph& graph, const std::vector<int>& source_flows, const std::vector<int>& sink_flows,
         int start) {
    int source_sum = 0;
    int sink_sum = 0;

    std::queue<int> queue;
    queue.push(start);
    graph.nodes_[start].visited = true;

    while (!queue.empty()) {
        int current = queue.front();
        queue.pop();

        if (current < static_cast<int>(source_flows.size())) {
            // source
            source_sum += source_flows[current];
        } else {
            // sink
            sink_sum += sink_flows[current - static_cast<int>(source_flows.size())];
        }

        for (int i = 0; i < static_cast<int>(graph.nodes_[current].next.size()); ++i) {
            int next = graph.nodes_[current].next[i];
            if (graph.nodes_[next].visited) {
                continue;
            }
            queue.push(next);
            graph.nodes_[next].visited = true;
        }
    }

    return std::min(source_sum, sink_sum);
}

void Solve(std::istream& is, std::ostream& os) {
    int n, k;
    is >> n >> k;

    std::vector<int> source_flows(n);
    std::vector<int> sink_flows(n);
    for (int i = 0; i < n; ++i) {
        is >> source_flows[i];
    }
    for (int i = 0; i < n; ++i) {
        is >> sink_flows[i];
    }

    // [0, n) - a
    // [n, 2 * n) - b
    Graph graph(2 * n);

    for (int i = 0; i < k; ++i) {
        int u;
        int v;
        is >> u >> v;
        graph.AddEdge(u - 1, n + v - 1);
    }

    int result = 0;
    for (int i = 0; i < 2 * n; ++i) {
        if (!graph.nodes_[i].visited) {
            result += Bfs(graph, source_flows, sink_flows, i);
        }
    }

    os << result;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    Solve(std::cin, std::cout);

    std::cout << std::flush;
    return 0;
}
