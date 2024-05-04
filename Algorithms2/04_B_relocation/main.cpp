#include <vector>
#include <iostream>
#include <list>
#include <queue>
#include <cassert>

const int kInf = 2000000000;

// A structure to represent a edge between
// two vertex
struct Edge {
    int next;  // Vertex next (or "to" vertex)
               // of a directed edge u-next. "From"
               // vertex u can be obtained using
               // index in adjacent array.

    int flow;  // flow of data in edge

    int max_flow;  // max_flow

    int reverse_edge_index;  // To store index of reverse
                             // edge in adjacency list so that
                             // we can quickly find it.
};

// Residual Graph
class Graph {
    int number_of_vertex_;
    std::vector<int> level_;
    std::vector<std::vector<Edge>> nodes_;

public:
    Graph(int number_of_vertex) {
        number_of_vertex_ = number_of_vertex;
        nodes_ = std::vector<std::vector<Edge>>(number_of_vertex);
        level_ = std::vector<int>(number_of_vertex);
    }

    // add edge to the graph
    void AddEdge(int u, int v, int c, bool directed = true) {
        int v_back_index = static_cast<int>(nodes_[v].size());
        int u_back_index = static_cast<int>(nodes_[u].size());

        // Forward edge : 0 flow and max_flow max_flow
        nodes_[u].push_back({v, 0, c, v_back_index});

        // Back edge : 0 flow and 0 max_flow
        if (directed) {
            nodes_[v].push_back({u, 0, 0, u_back_index});
        } else {
            nodes_[v].push_back({u, 0, c, u_back_index});
        }
    }

    // Finds if more flow can be sent from s to t.
    // Also assigns levels to nodes.
    bool BFS(int s, int t) {
        for (int i = 0; i < number_of_vertex_; i++) {
            level_[i] = -1;
        }

        level_[s] = 0;  // Level of source vertex

        // Create a queue, enqueue source vertex
        // and mark source vertex as visited here
        // level[] array works as visited array also.
        std::queue<int> q;
        q.push(s);

        std::vector<Edge>::iterator i;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (i = nodes_[u].begin(); i != nodes_[u].end(); i++) {
                Edge& e = *i;
                if (level_[e.next] < 0 && e.flow < e.max_flow) {
                    // Level of current vertex is,
                    // level of parent + 1
                    level_[e.next] = level_[u] + 1;

                    q.push(e.next);
                }
            }
        }

        // IF we can not reach to the sink we
        // return false else true
        return level_[t] >= 0;
    }

    // A DFS based function to send flow after BFS has
    // figured out that there is a possible flow and
    // constructed levels. This function called multiple
    // times for a single call of BFS.
    // flow : Current flow send by parent function call
    // start[] : To keep track of next edge to be explored.
    //		 start[i] stores count of edges explored
    //		 from i.
    // u : Current vertex
    // t : Sink
    int SendFlow(int u, int flow, int t, std::vector<int>& start) {
        // Sink reached
        if (u == t) {
            return flow;
        }

        // Traverse all adjacent edges one -by - one.
        for (; start[u] < static_cast<int>(nodes_[u].size()); start[u]++) {
            // Pick next edge from adjacency list of u
            Edge& e = nodes_[u][start[u]];

            if (level_[e.next] == level_[u] + 1 && e.flow < e.max_flow) {
                // find minimum flow from u to t
                int curr_flow = std::min(flow, e.max_flow - e.flow);

                int temp_flow = SendFlow(e.next, curr_flow, t, start);

                // flow is greater than zero
                if (temp_flow > 0) {
                    // add flow to current edge
                    e.flow += temp_flow;

                    // subtract flow from reverse edge
                    // of current edge
                    nodes_[e.next][e.reverse_edge_index].flow -= temp_flow;
                    return temp_flow;
                }
            }
        }

        return 0;
    }

    // Returns maximum flow in graph
    int DinicMaxflow(int s, int t) {
        // Corner case
        if (s == t) {
            return -1;
        }

        int total = 0;  // Initialize result

        // Augment the flow while there is path
        // from source to sink
        while (BFS(s, t)) {
            // store how many edges are visited
            // from number_of_vertex { 0 to number_of_vertex }

            // int *start = new int[number_of_vertex_ + 1]{0};
            std::vector<int> start(number_of_vertex_ + 1, 0);
            // while flow is not zero in graph from S to D
            while (int flow = SendFlow(s, kInf, t, start)) {

                // Add path flow to overall flow
                total += flow;
            }
        }

        // return maximum flow
        return total;
    }
};

void Test1() {
    {
        Graph g(6);

        g.AddEdge(0, 1, 16);
        g.AddEdge(0, 2, 13);
        g.AddEdge(1, 2, 10);
        g.AddEdge(1, 3, 12);
        g.AddEdge(2, 1, 4);
        g.AddEdge(2, 4, 14);
        g.AddEdge(3, 2, 9);
        g.AddEdge(3, 5, 20);
        g.AddEdge(4, 3, 7);
        g.AddEdge(4, 5, 4);

        assert(g.DinicMaxflow(0, 5) == 23);
    }
    {
        Graph g(6);

        g.AddEdge(0, 1, 3);
        g.AddEdge(0, 2, 7);
        g.AddEdge(1, 3, 9);
        g.AddEdge(1, 4, 9);
        g.AddEdge(2, 1, 9);
        g.AddEdge(2, 4, 9);
        g.AddEdge(2, 5, 4);
        g.AddEdge(3, 5, 3);
        g.AddEdge(4, 5, 7);
        g.AddEdge(0, 4, 10);

        assert(g.DinicMaxflow(0, 5) == 14);
    }
    {
        Graph g(6);

        g.AddEdge(0, 1, 10);
        g.AddEdge(0, 2, 10);
        g.AddEdge(1, 3, 4);
        g.AddEdge(1, 4, 8);
        g.AddEdge(1, 2, 2);
        g.AddEdge(2, 4, 9);
        g.AddEdge(3, 5, 10);
        g.AddEdge(4, 3, 6);
        g.AddEdge(4, 5, 10);

        assert(g.DinicMaxflow(0, 5) == 19);
    }
}

void TestUnoriented() {
    {
        Graph g(6);
        g.AddEdge(0, 1, 50);
        g.AddEdge(0, 2, 1);
        g.AddEdge(3, 5, 10);
        g.AddEdge(4, 5, 10);

        g.AddEdge(1, 3, kInf, false);
        g.AddEdge(2, 3, kInf, false);
        g.AddEdge(2, 4, kInf, false);

        assert(g.DinicMaxflow(0, 5) == 20);
    }
}

void TestLarge() {
    {
        int n = 50000;
        Graph g(2 * n + 2);
        int s = 0;
        int t = 1;

        g.AddEdge(s, 2, 1 + 1000);
        for (int i = 1; i < n; ++i) {
            g.AddEdge(s, 2 + i, 1);
        }

        for (int i = 0; i < n - 1; ++i) {
            g.AddEdge(2 + n + i, t, 1);
        }
        g.AddEdge(2 + n + n - 1, t, 1 + 1000);

        g.AddEdge(2, 2 + n, kInf, false);
        for (int i = 1; i < n - 1; ++i) {
            int u = i + 1;
            for (int j = -1; j < 2; ++j) {
                int v = u + j;
                g.AddEdge(2 + u - 1, 2 + n + v - 1, kInf, false);
            }
        }
        g.AddEdge(2 + n - 1, 2 + n + n - 1, kInf, false);

        int res = g.DinicMaxflow(s, t);
        assert(res == n + 1000);
    }
}

void Test() {
    Test1();
    TestUnoriented();
    TestLarge();
}

int BinarySearch(const Graph& graph, int n, int s, int t, int source_sum, int answer_low, int answer_high) {
    int last_correct_answer = kInf;
    while (answer_low < answer_high) {
        int answer = (answer_low + answer_high) / 2;
        Graph graph_copy = graph;

        for (int i = 0; i < n; ++i) {
            graph_copy.AddEdge(2 + i, t, answer);
        }

        int max_flow = graph_copy.DinicMaxflow(s, t);
        if (max_flow < source_sum) {
            answer_low = answer + 1;
        } else {
            last_correct_answer = answer;
            answer_high = answer;
        }
    }
    return last_correct_answer;
}

void Solve(std::istream& is, std::ostream& os) {
    int n, k;
    is >> n >> k;

    // 0 - s
    // 1 - t
    // [2, 2 + n) - a
    Graph graph(n + 2);
    int s = 0;
    int t = 1;
    int source_sum = 0;
    int source_max = 0;

    for (int i = 0; i < n; ++i) {
        int a;
        is >> a;
        graph.AddEdge(s, 2 + i, a);

        source_sum += a;
        if (a > source_max) {
            source_max = a;
        }
    }

    for (int i = 0; i < k; ++i) {
        int u;
        int v;
        is >> u >> v;
        graph.AddEdge(2 + u - 1, 2 + v - 1, kInf);
    }

    os << BinarySearch(graph, n, s, t, source_sum, source_sum / n, source_max + 1);
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    //Test();
    Solve(std::cin, std::cout);

    std::cout << std::flush;
    return 0;
}
