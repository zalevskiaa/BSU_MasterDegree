#include <iostream>

#include <vector>
#include <cmath>

struct Hole {
    int64_t value;
    int next;
    int count;
};

void Prep(std::vector<Hole>& holes, int block_size, int n, int a = -100000, int limit = 0) {
    if (a == -100000) {
        a = n - 1;
    }
    for (int i = a; i >= limit; --i) {
        int64_t next = i + holes[i].value;

        if (next < n) {
            if (next / block_size != i / block_size) {
                holes[i].next = next;
                holes[i].count = 1;
                continue;
            }

            holes[i].next = holes[next].next;
            holes[i].count = holes[next].count + 1;
            continue;
        }

        holes[i].next = -1;
        holes[i].count = 1;
    }
}

void Op0(std::vector<Hole>& holes, int block_size, int n, int a, int b) {
    int limit = a / block_size * block_size;
    holes[a].value = b;

    Prep(holes, block_size, n, a, limit);
}

void Op1(std::vector<Hole>& holes, int& count, int& j) {
    Hole hole = holes[j];

    while (true) {
        if (hole.next == -1) {
            break;
        }
        count += hole.count;
        j = hole.next;
        hole = holes[j];
    }

    while (true) {
        if (hole.count == 1) {
            break;
        }
        j += hole.value;
        hole = holes[j];
        count += 1;
    }
}

int main() {
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    std::ios_base::sync_with_stdio(0);

    int n;
    int m;
    std::cin >> n >> m;
    std::vector<Hole> holes(n, {0, -1, 1});

    int block_size = sqrt(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> holes[i].value;
    }

    Prep(holes, block_size, n);

    int op, a, b;
    for (int i = 0; i < m; ++i) {
        std::cin >> op;

        if (op == 0) {
            std::cin >> a >> b;

            Op0(holes, block_size, n, a - 1, b);
            continue;
        }

        if (op == 1) {
            std::cin >> a;

            int count = 1;
            int j = a - 1;

            Op1(holes, count, j);
            std::cout << j + 1 << " " << count << '\n';
            continue;
        }

        throw "lol";
    }

    return 0;
}