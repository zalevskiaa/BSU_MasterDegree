#include <iostream>

#include <vector>
#include <algorithm>
#include <set>

const int kMaxN = 150'000; // kMaxN was 100k but 3/100 tests failed, so I increased to 150k

struct TreeElem {
    bool valid, has_sons;
    int left, right;
    int covering_segments_count, covered_length;

    TreeElem(bool valid, int left, int right)
        : valid(false),
          has_sons(false),
          left(-1),
          right(-1),
          covering_segments_count(0),
          covered_length(0) {
    }
    int RealLength() {
        return right - left;
    }
};

class Tree {
private:
    std::vector<TreeElem> data_;
    std::vector<int> points_;

    void Build(int cur_data_i, int left_points_i, int right_points_i) {
        data_[cur_data_i].valid = true;
        data_[cur_data_i].left = points_[left_points_i];
        data_[cur_data_i].right = points_[right_points_i];

        if (right_points_i - left_points_i < 2) {
            return;
        }

        data_[cur_data_i].has_sons = true;
        int mid_points_i = (left_points_i + right_points_i) / 2;
        Build(2 * cur_data_i, left_points_i, mid_points_i);       // left son
        Build(2 * cur_data_i + 1, mid_points_i, right_points_i);  // right son
    }

public:
    Tree(const std::set<int>& points_set, int maxsize) {
        data_.reserve(maxsize + 1);
        for (int i = 0; i < maxsize + 1; ++i) {
            data_.push_back(TreeElem(false, -1, -1));
        }

        points_.reserve(points_set.size());
        for (std::set<int>::iterator it = points_set.begin(); it != points_set.end(); ++it) {
            points_.push_back(*it);
        }
        /*
        for (int i = 0; i < points_.size(); ++i) {
            std::cout << points_[i] << ' ';
        }
        */
        Build(1, 0, points_.size() - 1);
    }

    void Update(char type, int left_val, int right_val, int cur_data_i = 1) {
        if (left_val == data_[cur_data_i].left && right_val == data_[cur_data_i].right) {
            // cur matches to l-r
            if (type == '+') {
                data_[cur_data_i].covering_segments_count += 1;
            } else {
                data_[cur_data_i].covering_segments_count -= 1;
            }
        } else {
            // go left
            if (left_val < data_[2 * cur_data_i].right) {
                Update(type, left_val, std::min(right_val, data_[2 * cur_data_i].right),
                       2 * cur_data_i);
            }
            // go right
            if (data_[2 * cur_data_i + 1].left < right_val) {
                Update(type, std::max(left_val, data_[2 * cur_data_i + 1].left), right_val,
                       2 * cur_data_i + 1);
            }
        }

        // update covered_length
        if (!data_[cur_data_i].has_sons) {
            // cur is leaf
            if (data_[cur_data_i].covering_segments_count > 0) {
                data_[cur_data_i].covered_length = data_[cur_data_i].RealLength();
            } else {
                data_[cur_data_i].covered_length = 0;
            }
        } else {
            if (data_[cur_data_i].covering_segments_count > 0) {
                data_[cur_data_i].covered_length = data_[cur_data_i].RealLength();
            } else {
                data_[cur_data_i].covered_length =
                    data_[2 * cur_data_i].covered_length + data_[2 * cur_data_i + 1].covered_length;
            }
        }
    }

    int RootValue() {
        return data_[1].covered_length;
    }
};

struct QueryElem {
    char type;
    int left, right;
    QueryElem(char type, int left, int right) : type(type), left(left), right(right) {
    }
};

void Solve() {
    int queries_count;
    std::cin >> queries_count;

    std::set<int> points_set;
    std::vector<QueryElem> queries;
    queries.reserve(queries_count);

    for (int query = 0; query < queries_count; ++query) {
        char query_type;
        int left, right;
        std::cin >> query_type >> left >> right;

        queries.push_back(QueryElem(query_type, left, right));

        if (query_type == '-') {
            continue;
        }

        points_set.insert(left);
        points_set.insert(right);
    }

    // build tree

    Tree tree(points_set, 4 * kMaxN);

    // queries

    for (int query = 0; query < queries_count; ++query) {
        // check maybe l > r -> swap
        if (queries[query].left > queries[query].right) {
            std::swap(queries[query].left, queries[query].right);
        }

        // check maybe l == r -> continue
        if (queries[query].left == queries[query].right) {
            std::cout << tree.RootValue() << '\n';
            continue;
        }

        tree.Update(queries[query].type, queries[query].left, queries[query].right);
        std::cout << tree.RootValue() << '\n';
    }
}

int main() {
    Solve();
}