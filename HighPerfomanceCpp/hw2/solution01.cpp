#include <bits/stdc++.h>

using namespace std;

typedef long long ll;

class SegmentTree {
public:

    explicit SegmentTree(const vector<int> &vals) {
        int n = (int) vals.size();
        tree.resize(4 * n);
        Build(vals, 1, 0, n);
    }

    void Set(int idx, int val, int vert, int left, int right) {
        Push(vert, left, right);

        if (idx >= right || idx < left) {
            return;
        }

        if (left + 1 == right) {
            tree[vert] = Node(val);
            return;
        }


        int md = (left + right) / 2;
        Set(idx, val, 2 * vert, left, md);
        Set(idx, val, 2 * vert + 1, md, right);
        tree[vert].Merge(tree[2 * vert], tree[2 * vert + 1]);
    }

    void Inc(int l, int r, int vert, int left, int right) {
        if (l <= left && r >= right) {
            tree[vert].push_add++;
            Push(vert, left, right);
            return;
        }
        Push(vert, left, right);

        if (l >= right || r <= left) {
            return;
        }

        int md = (left + right) / 2;
        Inc(l, r, 2 * vert, left, md);
        Inc(l, r, 2 * vert + 1, md, right);
        tree[vert].Merge(tree[2 * vert], tree[2 * vert + 1]);
    }

    ll GetSumOdd(int l, int r, int vert, int left, int right) {
        if (l >= right || r <= left) {
            return 0;
        }
        Push(vert, left, right);
        if (l <= left && r >= right) {
            return tree[vert].sum_odd;
        }
        int md = (left + right) / 2;
        return GetSumOdd(l, r, 2 * vert, left, md) + GetSumOdd(l, r, 2 * vert + 1, md, right);
    }

    ll GetSumEven(int l, int r, int vert, int left, int right) {
        if (l >= right || r <= left) {
            return 0;
        }
        Push(vert, left, right);
        if (l <= left && r >= right) {
            return tree[vert].sum_even;
        }
        int md = (left + right) / 2;
        return GetSumEven(l, r, 2 * vert, left, md) + GetSumEven(l, r, 2 * vert + 1, md, right);
    }

private:
    struct Node {
        ll sum_odd;
        ll sum_even;
        int cnt_odd;
        int push_add;

        Node() = default;

        explicit Node(int val) {
            if (val % 2 == 1) {
                sum_odd = val;
                sum_even = 0;
                cnt_odd = 1;
                push_add = 0;
            } else {
                sum_odd = 0;
                sum_even = val;
                cnt_odd = 0;
                push_add = 0;
            }
        }

        inline void Merge(const Node &lhs, const Node &rhs) {
            sum_odd = lhs.sum_odd + rhs.sum_odd;
            sum_even = lhs.sum_even + rhs.sum_even;
            cnt_odd = lhs.cnt_odd + rhs.cnt_odd;
            push_add = 0;
        }
    };

    inline void swap(ll& a, ll& b) {
        ll c = a;
        a = b;
        b = c;
    }
    inline void swap(int& a, int& b) {
        int c = a;
        a = b;
        b = c;
    }

    inline void Push(int vert, int left, int right) {
        Node& node = tree[vert];

        ll add = node.push_add;
        if (add) {
            int &cnt_odd(node.cnt_odd);
            int cnt_even = right - left - cnt_odd;
            if (add & 1) {
                swap(node.sum_odd, node.sum_even);
                swap(cnt_odd, cnt_even);
            }

            // ll ll_add = 1LL * add;
            node.sum_odd += add * cnt_odd;
            node.sum_even += add * cnt_even;
            if (left + 1 < right) {
                vert *= 2;
                tree[vert].push_add += add;

                vert++;
                tree[vert].push_add += add;
            }
            node.push_add = 0;
        }
    }

    void Build(const vector<int> &vals, int vert, int left, int right) {
        if (left + 1 == right) {
            tree[vert] = Node(vals[left]);
            return;
        }
        int md = (left + right) / 2;
        Build(vals, 2 * vert, left, md);
        Build(vals, 2 * vert + 1, md, right);
        tree[vert].Merge(tree[2 * vert], tree[2 * vert + 1]);
    }

    vector<Node> tree;
};

struct Request {
    int type;
    int left, right;

    Request(mt19937 &gen, int n) {
        type = gen() % 4 + 1;
        right = gen() % (n - 1) + 1;
        left = gen() % right + 1;
    }
};


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
#ifdef ONPC
    freopen("input.txt", "r", stdin);
//    freopen("output.txt", "w", stdout);
#endif
    int size, cnt_request;
    cin >> size >> cnt_request;
    vector<int> vals(size);
    for (int i = 0; i < size; ++i) {
        cin >> vals[i];
    }
    SegmentTree algo(vals);
#ifdef ONPC
    mt19937 gen(0);
    vector<Request> a;
    for (int i = 0; i < cnt_request; ++i) {
        a.push_back(Request(gen, size));
    }
    ifstream fin("output.txt");
#endif
    for (int request_idx = 0; request_idx < cnt_request; ++request_idx) {
        ll ans = -1, result;
#ifdef ONPC
        auto[type, left, right] = a[request_idx];
//        int type, left, right;
//        cin >> type >> left >> right;
        if (type == 1) {
            vals[left - 1] = right;
        } else if (type == 2) {
            for (int i = left - 1; i < right; ++i) {
                vals[i]++;
            }
        } else if (type == 3) {
            ans = 0;
            assert(left <= right);
            for (int i = left - 1; i < right; ++i) {
                if (vals[i] % 2 == 0) {
                    ans += vals[i];
                }
            }
        } else {
            ans = 0;
            assert(type == 4);
            for (int i = left - 1; i < right; ++i) {
                if (vals[i] % 2 == 1) {
                    ans += vals[i];
                }
            }
        }
#else

        int type, left, right;
        cin >> type >> left >> right;
#endif
        if (type == 1) {
            algo.Set(left - 1, right, 1, 0, size);
        } else if (type == 2) {
            algo.Inc(left - 1, right, 1, 0, size);
        } else if (type == 3) {
            result = algo.GetSumEven(left - 1, right, 1, 0, size);
            cout << result << "\n";
        } else {
//            assert(type == 4);
            result = algo.GetSumOdd(left - 1, right, 1, 0, size);
            cout << result << "\n";
        }
        assert(ans == -1 || result == ans);
    }
    return 0;
}