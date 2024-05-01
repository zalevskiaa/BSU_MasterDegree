#include <iostream>
#include <cassert>
#include <vector>
#include <bitset>
#include <queue>

int64_t DistancesSumSlow(const std::vector<std::vector<bool>> &matrix) {
    const int inf = 1e9;

    int n = matrix.size();
    std::vector<std::vector<int>> distances(n, std::vector<int>(n, inf));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == j) {
                distances[i][j] = 0;
                continue;
            }
            if (matrix[i][j]) {
                distances[i][j] = 1;
            }
        }
    }

    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                distances[i][j] = std::min(distances[i][j], distances[i][k] + distances[k][j]);
            }
        }
    }

    int64_t result = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (distances[i][j] == inf) {
                distances[i][j] = n;
            }
            result += distances[i][j] * distances[i][j];
        }
    }
    return result;
}

const int kNaxn = 2022;

struct QueueItem {
    size_t vertex;
    size_t distance;
};

int64_t DistancesSumFromVertex(const std::vector< std::bitset<kNaxn> > &matrix, size_t u) {
    int64_t result = 0;

    size_t n = matrix.size();
    std::bitset<kNaxn> unvisited;
    unvisited.flip();

    std::queue<QueueItem> queue;
    queue.push({u, 0});
    unvisited[u] = false;

    size_t unreachable_count = n;

    while (!queue.empty()) {
        QueueItem current = queue.front();
        queue.pop();

        result += current.distance * current.distance;
        --unreachable_count;

        std::bitset<kNaxn> news = matrix[current.vertex] & unvisited;

        for (size_t next = news._Find_first(); next < n; next = news._Find_next(next)) {
            queue.push({next, current.distance + 1});
            unvisited[next] = false;
        }
    }

    result += static_cast<int64_t>(unreachable_count) * n * n;

    return result;
}

int64_t DistancesSum(const std::vector< std::bitset<kNaxn> > &matrix) {
    int n = matrix.size();
    int64_t result = 0;

    for (size_t i = 0; i < n; ++i) {
        result += DistancesSumFromVertex(matrix, i);
    }

    return result;
}


void Test() {
    int n = 7;
    std::vector< std::bitset<kNaxn> > bs(n);
    std::vector<std::vector<bool>> matrix(n, std::vector<bool>(n));

    srand(2022);

    for (int t = 0; t < 100000; ++t) {
        int i = rand() % n;
        int j = rand() % n;
        bs[i].flip(j);
        matrix[i][j] = bs[i][j];

        int64_t expected = DistancesSumSlow(matrix);
        int64_t actual = DistancesSum(bs);

        if (expected != actual) {
            std::cout << expected << " " << actual << "\n";
            return;
        }
    }
}

void Debug() {

}

void Solve() {
    int n;
    std::cin >> n;

    //std::vector<std::vector<bool>> matrix(n, std::vector<bool>(n));
    std::vector< std::bitset<kNaxn> > matrix(n);
    for (int i = 0; i < n; ++i) {
        std::string row;
        std::cin >> row;
        for (int j = 0; j < n; ++j) {
            matrix[i][j] = (row[j] == '1');
        }
    }
    std::cout << DistancesSum(matrix);

}

int main() {
//    Test();
//    Debug();
    Solve();

    return 0;
}
