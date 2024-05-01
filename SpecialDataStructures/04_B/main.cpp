#include <iostream>
#include <vector>

struct Node {
    int value;
    int add;
    bool leaf;
};

class Tree {
public:
    Tree(int size) : data_(std::vector<Node>(size, {0, 0, false})) {}

    void Build(int value, int t_left, int t_right) {
        if (t_left == t_right) {
            data_[value].leaf = true;
            return;
        }
        int mid = (t_left + t_right) / 2;
        int v2 = 2 * value;

        Build(v2, t_left, mid);
        Build(v2 + 1, mid + 1, t_right);
    }

    int GetMax(int value, int t_left, int t_right, int left, int right) {
        if (left > right) {
            return 0;
        }

        if (left == t_left && right == t_right) {
            Push(value);
            return data_[value].value;

        } else {
            Push(value);

            int half = (t_left + t_right) / 2;
            int v2 = value * 2;

            int sum_left = GetMax(v2, t_left, half, left, std::min(right, half));
            int sum_right = GetMax(v2 + 1, half + 1, t_right, std::max(left, half + 1), right);

            return std::max(sum_left, sum_right);
        }
    }

    void Increase(int value, int t_left, int t_right, int left, int right) {
        if (left > right) {
            Push(value);
            return;
        }

        if (left == t_left && right == t_right) {
            data_[value].add++;
            Push(value);

        } else {
            Push(value);

            int half = (t_left + t_right) / 2;
            int v2 = value * 2;

            Increase(v2, t_left, half, left, std::min(right, half));
            Increase(v2 + 1, half + 1, t_right, std::max(left, half + 1), right);

            data_[value].value = std::max(data_[v2].value, data_[v2 + 1].value);
        }
    }

private:
    void Push(int value) {
        if (!data_[value].leaf) {
            data_[value * 2].add += data_[value].add;
            data_[value * 2 + 1].add += data_[value].add;
        }

        data_[value].value += data_[value].add;
        data_[value].add = 0;
    }

    std::vector<Node> data_;
};

void Debug() {

}

void Solve() {
    int stations_number, seats_number, queries_count;
    std::cin >> stations_number >> seats_number >> queries_count;

    int tree_size = 1;
    for (; tree_size < 2 * stations_number; tree_size <<= 1) {
        //std::cout << tree_size << " ";
    }
    Tree tree(tree_size);

    tree.Build(1, 0, stations_number - 1);

    int left, right;
    for (int i = 0; i < queries_count; ++i) {
        std::cin >> left >> right;
        int sum = tree.GetMax(1, 0, stations_number - 1, left, right - 1);

        if (sum < seats_number) {
            std::cout << 1 << '\n';
            tree.Increase(1, 0, stations_number - 1, left, right - 1);
        } else {
            std::cout << 0 << '\n';
        }

//        std::cout << std::flush;
    }
}

int main() {
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    std::ios_base::sync_with_stdio(0);

//    Debug();
    Solve();

    return 0;
}