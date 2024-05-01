#include <iostream>
#include <vector>

struct TreeElem {
    int64_t odd_n = 0;
    int64_t odd_sum = 0;
    int64_t even_n = 0;
    int64_t even_sum = 0;
    int64_t add = 0;
};
TreeElem operator+(const TreeElem& left, const TreeElem& right) {
    return {left.odd_n + right.odd_n, left.odd_sum + right.odd_sum, left.even_n + right.even_n,
            left.even_sum + right.even_sum, 0};
}
class SegmentTree {
public:
    SegmentTree(int size) : size_(size), data_(std::vector<TreeElem>(8 * size)) {
    }
    SegmentTree(const std::vector<int>& values) : SegmentTree(values.size()) {
        PBuild(values, 1, 0, size_ - 1);
    }

    void Update(int pos, int val) {
        return PUpdate(1, 0, size_ - 1, pos, val);
    }
    void AddOne(int left, int right) {
        PAddOne(1, 0, size_ - 1, left, right);
    }
    int64_t SumOdd(int left, int right) {
        return PSumOdd(1, 0, size_ - 1, left, right);
    }
    int64_t SumEven(int left, int right) {
        return PSumEven(1, 0, size_ - 1, left, right);
    }

private:
    void PBuild(const std::vector<int>& values, int current, int index_l, int index_r) {
        if (index_l == index_r) {
            if (IsOdd(values[index_l])) {
                data_[current] = {1, values[index_l], 0, 0, 0};
            } else {
                data_[current] = {0, 0, 1, values[index_l], 0};
            }
        } else {
            int index_m = (index_l + index_r) / 2;
            PBuild(values, 2 * current, index_l, index_m);
            PBuild(values, 2 * current + 1, index_m + 1, index_r);
            data_[current] = data_[2 * current] + data_[2 * current + 1];
        }
    }
    void PUpdate(int current, int index_l, int index_r, int query_pos, int query_val) {
        if (index_l == index_r) {
            if (IsOdd(query_val)) {
                data_[current] = {1, query_val, 0, 0, 0};
            } else {
                data_[current] = {0, 0, 1, query_val, 0};
            }
        } else {
            Push(current);
            int index_m = (index_l + index_r) / 2;
            if (query_pos <= index_m) {
                PUpdate(current * 2, index_l, index_m, query_pos, query_val);
            } else {
                PUpdate(current * 2 + 1, index_m + 1, index_r, query_pos, query_val);
            }
            Push(2 * current);
            Push(2 * current + 1);
            data_[current] = data_[current * 2] + data_[current * 2 + 1];
        }
    }
    void PAddOne(int current, int index_l, int index_r, int query_l, int query_r) {
        if (query_l > query_r) {
            return;
        }
        if (query_l == index_l && query_r == index_r) {
            data_[current].add += 1;
        } else {
            Push(current);
            int index_m = (index_l + index_r) / 2;
            PAddOne(current * 2, index_l, index_m, query_l, std::min(query_r, index_m));
            PAddOne(current * 2 + 1, index_m + 1, index_r, std::max(query_l, index_m + 1), query_r);

            Push(2 * current);
            Push(2 * current + 1);
            data_[current] = data_[current * 2] + data_[current * 2 + 1];
        }
    }
    void Push(int current) {
        if (data_[current].add > 0) {
            data_[current].odd_sum += data_[current].odd_n * data_[current].add;
            data_[current].even_sum += data_[current].even_n * data_[current].add;
            if (IsOdd(data_[current].add)) {
                std::swap(data_[current].odd_n, data_[current].even_n);
                std::swap(data_[current].odd_sum, data_[current].even_sum);
            }
            data_[2 * current].add += data_[current].add;
            data_[2 * current + 1].add += data_[current].add;
            data_[current].add = 0;
        }
    }
    int64_t PSumOdd(int current, int index_l, int index_r, int query_l, int query_r) {
        if (query_l > query_r) {
            return 0;
        }
        Push(current);
        if (query_l == index_l && query_r == index_r) {
            return data_[current].odd_sum;
        }

        int index_m = (index_l + index_r) / 2;
        return PSumOdd(current * 2, index_l, index_m, query_l, std::min(query_r, index_m)) +
               PSumOdd(current * 2 + 1, index_m + 1, index_r, std::max(query_l, index_m + 1),
                       query_r);
    }
    int64_t PSumEven(int current, int index_l, int index_r, int query_l, int query_r) {
        if (query_l > query_r) {
            return 0;
        }
        Push(current);
        if (query_l == index_l && query_r == index_r) {
            return data_[current].even_sum;
        }

        int index_m = (index_l + index_r) / 2;
        return PSumEven(current * 2, index_l, index_m, query_l, std::min(query_r, index_m)) +
               PSumEven(current * 2 + 1, index_m + 1, index_r, std::max(query_l, index_m + 1),
                        query_r);
    }
    static inline bool IsOdd(int64_t value) {
        return value & 1;
    }

    int size_;
    std::vector<TreeElem> data_;
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, q;
    std::cin >> n >> q;
    std::vector<int> values(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> values[i];
    }
    SegmentTree tree(values);

    for (int i = 0; i < q; ++i) {
        int c, l, r;
        std::cin >> c >> l >> r;
        switch (c) {
            case 1:
                tree.Update(l - 1, r);
                break;
            case 2:
                tree.AddOne(l - 1, r - 1);
                break;
            case 3:
                std::cout << tree.SumEven(l - 1, r - 1) << '\n';
                break;
            case 4:
                std::cout << tree.SumOdd(l - 1, r - 1) << '\n';
                break;
        }
    }

    std::cout << std::flush;
    return 0;
}
