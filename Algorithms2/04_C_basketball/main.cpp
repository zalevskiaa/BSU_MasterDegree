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

bool Solve(int n, const std::vector<int>& current_wins, const std::vector<int>& future_matches_total,
           const std::vector<std::vector<int>>& future_matches_div) {
    if (n < 2) {
        // impossible, but just in case
        return true;
    }

    // the favourite team always wins
    int favourite_current_wins = current_wins[0] + future_matches_total[0];

    if (n == 2) {
        return favourite_current_wins >= current_wins[1];
    }

    int non_favourite_teams_number = n - 1;
    int non_favourite_pairs_number = (n - 1) * (n - 2) / 2;

    // [0, nfpn)         -- first layer; for every pair of non-favourite teams
    // [nfpn, nfpn + nftn) -- second layer; for every non-favourite team
    // nfpn + nftn       -- source node
    // nfpn + nftn + 1   -- sink node
    Graph graph(non_favourite_teams_number + non_favourite_pairs_number + 2);
    int s = non_favourite_teams_number + non_favourite_pairs_number;
    int t = non_favourite_teams_number + non_favourite_pairs_number + 1;

    int source_flow = 0;

    int pair_index = 0;
    for (int x = 1; x < n; ++x) {
        int x_index = non_favourite_pairs_number + x - 1;

        for (int y = x + 1; y < n; ++y, ++pair_index) {
            int y_index = non_favourite_pairs_number + y - 1;
            int c = future_matches_div[x][y];
            if (c == 0) {
                continue;
            }

            source_flow += c;

            graph.AddEdge(s, pair_index, c); // source -> pair
            graph.AddEdge(pair_index, x_index, c); // pair -> x
            graph.AddEdge(pair_index, y_index, c); // pair -> y
        }
    }
    assert(pair_index == non_favourite_pairs_number);

    for (int i = 1; i < n; ++i) {
        int team_index = non_favourite_pairs_number + i - 1;

        if (favourite_current_wins < current_wins[i]) {
            // no way because past games are already greater than favourite's
            return false;
        }
        if (favourite_current_wins == current_wins[i]) {
            continue;
        }

        graph.AddEdge(team_index, t, favourite_current_wins - current_wins[i]); // team -> sink
    }

    int max_flow = graph.DinicMaxflow(s, t);
    return max_flow == source_flow;
}

//*

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;
    std::vector<int> current_wins(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> current_wins[i];
    }
    std::vector<int> future_matches_total(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> future_matches_total[i];
    }
    std::vector<std::vector<int>> future_matches_div(n, std::vector<int>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            std::cin >> future_matches_div[i][j];
        }
    }

    if (Solve(n, current_wins, future_matches_total, future_matches_div)) {
        std::cout << "YES";
    } else {
        std::cout << "NO";
    }

    std::cout << std::flush;
    return 0;
}

//*/