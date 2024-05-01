#include <iostream>
#include <vector>

struct TreeElem {
    int64_t w = 0;
    int64_t s = 0;
};

class SegmentTree {
public:
    SegmentTree(int size) : size_(size), data_(std::vector<TreeElem>(4 * size)) {
        PBuild(1, 0, size_ - 1);
    }

    void Update(int position, int duration) {
        return PUpdate(1, 0, size_ - 1, position, duration);
    }

    int64_t Ask(int left, int right) {
        return PAsk(1, 0, size_ - 1, left, right).s;
    }
    void Print(int current, int index_l, int index_r, std::string prefix = "") {
        std::cout << prefix << "[" << current << "][" << index_l << " " << index_r << "]   w " << data_[current].w << ",  s " << data_[current].s << '\n';
        if (index_l == index_r) {
            return;
        }

        int index_m = (index_l + index_r) / 2;
        Print( 2 * current, index_l, index_m, prefix + "    ");
        Print( 2 * current + 1, index_m + 1, index_r, prefix + "    ");
    }

private:
    void PBuild(int current, int index_l, int index_r) {
        if (index_l == index_r) {
            data_[current] = {0, index_l};
        } else {
            int index_m = (index_l + index_r) / 2;
            PBuild( 2 * current, index_l, index_m);
            PBuild( 2 * current + 1, index_m + 1, index_r);
            data_[current].w = data_[2 * current].w + data_[2 * current + 1].w;
            data_[current].s = std::max(data_[2 * current].s + data_[2 * current + 1].w, data_[2 * current + 1].s);
        }
    }
    void PUpdate(int current, int index_l, int index_r, int query_position, int query_duration) {
        if (index_l == index_r) {
            // leaf
            if (query_duration > 0) {
                // Join type
                data_[current] = {query_duration, query_position + query_duration};
            } else {
                // Cancel type
//                data_[current] = {0, 0};
                data_[current] = {0, query_position};
            }
            return;
        }
        int index_m = (index_l + index_r) / 2;
        if (query_position <= index_m) {
            PUpdate(current * 2, index_l, index_m, query_position, query_duration);
        } else {
            PUpdate(current * 2 + 1, index_m + 1, index_r, query_position, query_duration);
        }
        data_[current].w = data_[2 * current].w + data_[2 * current + 1].w;
        data_[current].s = std::max(data_[2 * current].s + data_[2 * current + 1].w, data_[2 * current + 1].s);
    }

    TreeElem PAsk(int current, int index_l, int index_r, int query_l, int query_r) {
        if (index_l > index_r) {
            return {0, 0};
        }
        if (query_l == index_l && query_r == index_r) {
            return data_[current];
        }

        int index_m = (index_l + index_r) / 2;
        if (query_r <= index_m) {
            TreeElem left = PAsk(current * 2, index_l, index_m, query_l, query_r);
            TreeElem right = {0, 0};
            return {left.w + right.w, std::max(left.s + right.w, right.s)};
        }

        if (query_l > index_m) {
            TreeElem left = {0, 0};
            TreeElem right = PAsk(current * 2 + 1, index_m + 1, index_r, query_l, query_r);
            return {left.w + right.w, std::max(left.s + right.w, right.s)};
        }

        TreeElem left = PAsk(current * 2, index_l, index_m, query_l, std::min(query_r, index_m));
        TreeElem right = PAsk(current * 2 + 1, index_m + 1, index_r, std::max(query_l, index_m + 1), query_r);
        return {left.w + right.w, std::max(left.s + right.w, right.s)};
    }

    int size_;
    std::vector<TreeElem> data_;
};

struct Query {
    char type;
    int lhs;
    int rhs;
};

void Debug() {
    int n = 5;
    SegmentTree tree(n);
    tree.Print(1, 0, n - 1);
    tree.Update(0, 4);
    tree.Print(1, 0, n - 1);
}

int main() {
    //Debug();
    //return 0;

    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 1'000'000 + 10;
    // n = 10;
    int q;
    std::cin >> q;
    std::vector<Query> queries(q);

    SegmentTree tree(n);

    for (int i = 0; i < q; ++i) {
        std::cin >> queries[i].type;
        int64_t s;
        switch (queries[i].type) {
            case '?':
                std::cin >> queries[i].lhs;
                s = tree.Ask(0, queries[i].lhs - 1);
                if (s > 0 || true) {
                    std::cout << s - queries[i].lhs + 1 << '\n';
                } else {
                    std::cout << 0 << '\n';
                }
                break;
            case '+':
                std::cin >> queries[i].lhs >> queries[i].rhs;
                tree.Update(queries[i].lhs - 1, queries[i].rhs);
                break;
            case '-':
                std::cin >> queries[i].lhs;
                if (queries[queries[i].lhs - 1].type != '+') {
                    throw std::runtime_error("bad query param");
                }
                tree.Update(queries[queries[i].lhs - 1].lhs - 1, 0);
                break;
            default:
                throw std::runtime_error("bad query");
        }
    }

    std::cout << std::flush;
    return 0;
}
