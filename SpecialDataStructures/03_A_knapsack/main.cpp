#include <iostream>
#include <cassert>
#include <vector>
#include <optional>

struct Item {
    int w, c;
};

struct Subset {
    std::vector<int> ids;
    int64_t w = 0;
    int64_t c = 0;
    bool bad = false;
};


std::vector<std::vector<bool>> MakeMasks(int n) {
    std::vector<std::vector<bool>> result;
    result.reserve(1 << n);

    std::vector<int> current(n + 1);
    while (!current[n]) {
        result.push_back(std::vector<bool>(n));
        for (int i = 0; i < n; ++i) {
            result.back()[i] = current[i];
        }

        ++current[0];
        for (int i = 0; i < n && current[i] > 1; ++i) {
            ++current[i + 1];
            current[i] = 0;
        }
    }

    assert(result.size() == 1 << n);
    return result;
}

std::vector<Subset> MakeSubsets(const std::vector<Item> &items, int n, int id_shift = 0) {
    std::vector<std::vector<bool>> masks = MakeMasks(n);
    std::vector<Subset> subsets(masks.size());

    for (size_t subset_index = 0; subset_index < masks.size(); ++subset_index) {
        for (int id = 0; id < n; ++id) {
            if (masks[subset_index][id]) {
                subsets[subset_index].w += items[id + id_shift].w;
                subsets[subset_index].c += items[id + id_shift].c;
                subsets[subset_index].ids.push_back(id + id_shift);
            }
        }
    }

    return subsets;
}

void MarkBad(std::vector<Subset> *subsets) {
    int64_t last_good_c = (*subsets)[0].c;

    for (int i = 1; i < subsets->size(); ++i) {
        if ((*subsets)[i].c <= last_good_c) {
            (*subsets)[i].bad = true;
        } else {
            last_good_c = (*subsets)[i].c;
        }
    }
}

void Print(const std::vector<Subset> &subsets, const std::string &end = "") {
    std::cout << "b w c\n";
    for (const auto &subset: subsets) {
        std::cout << subset.bad << ' ' << subset.w << ' ' << subset.c << ", ";
        for (const auto &id: subset.ids) {
            std::cout << id << ' ';
        }
        std::cout << '\n';
    }
    std::cout << end;
}

std::vector<int> Knapsack(const std::vector<Item> &items, int max_w) {
    int n1 = items.size() / 2;
    int n2 = items.size() - n1;

    std::vector<Subset> subsets_1 = MakeSubsets(items, n1);
    std::vector<Subset> subsets_2 = MakeSubsets(items, n2, n1);

    std::sort(subsets_1.begin(), subsets_1.end(), [](const Subset &a, const Subset &b) { return a.w < b.w; });
    MarkBad(&subsets_1);

    std::stable_partition(subsets_1.begin(), subsets_1.end(), [](const Subset &a) { return !a.bad; });
    while (subsets_1.back().bad) {
        subsets_1.pop_back();
    }

//    Print(subsets_1, "\n");
//    Print(subsets_2, "\n");

    std::optional<std::vector<Subset>::iterator> best_1, best_2;

    for (std::vector<Subset>::iterator iter_2 = subsets_2.begin(); iter_2 != subsets_2.end(); ++iter_2) {
//        auto lb = std::lower_bound(subsets_1.begin(), subsets_1.end(), max_w - subset_2.w,
//                                   [](const Subset &b, int64_t w) { return b.w < w; });
//        if (lb != subsets_1.begin()) {
//            --lb;
//        }
        auto iter_1 = std::upper_bound(subsets_1.begin(), subsets_1.end(), max_w - iter_2->w,
                                       [](int64_t w, const Subset &b) { return w < b.w; });
        if (iter_1 == subsets_1.begin()) {
            continue;
        }
        --iter_1;

        if (!best_1.has_value() || iter_1->c + iter_2->c > best_1.value()->c + best_2.value()->c) {
            best_1 = iter_1;
            best_2 = iter_2;
        }
    }

    std::vector<int> result;
    result.reserve(items.size());

    if (best_1.has_value()) {
        for (int elem: best_1.value()->ids) {
            result.push_back(elem);
        }
        for (int elem: best_2.value()->ids) {
            result.push_back(elem);
        }
        std::sort(result.begin(), result.end());
    }

    return result;
}

std::vector<int> SlowKnapsack(const std::vector<Item> &items, int max_w) {
    std::vector<Subset> subsets = MakeSubsets(items, items.size());
    std::optional<std::vector<Subset>::iterator> best;
    for (std::vector<Subset>::iterator iter = subsets.begin(); iter != subsets.end(); ++iter) {
        if (iter->w <= max_w && (!best.has_value() || iter->c > best.value()->c)) {
            best = iter;
        }
    }
    std::vector<int> result;
    result.reserve(items.size());

    if (best.has_value()) {
        for (int elem: best.value()->ids) {
            result.push_back(elem);
        }
    }

    return result;
}

void Test() {
    std::vector<Item> items(5);
    const int w0 = 1, w1 = 5, c0 = 1, c1 = 5;
    const int max_w0 = 1, max_w1 = 26;

    int64_t incorrect = 0;
    int64_t total = 0;

    for (int max_w = max_w0; max_w < max_w1; ++max_w) {
        std::cerr << '.';
        for (items[0].w = w0; items[0].w < w1; ++items[0].w) {
            for (items[0].c = c0; items[0].c < c1; ++items[0].c) {
                for (items[1].w = w0; items[1].w < w1; ++items[1].w) {
                    for (items[1].c = c0; items[1].c < c1; ++items[1].c) {
                        for (items[2].w = w0; items[2].w < w1; ++items[2].w) {
                            for (items[2].c = c0; items[2].c < c1; ++items[2].c) {
                                for (items[3].w = w0; items[3].w < w1; ++items[3].w) {
                                    for (items[3].c = c0; items[3].c < c1; ++items[3].c) {
                                        for (items[4].w = w0; items[4].w < w1; ++items[4].w) {
                                            for (items[4].c = c0; items[4].c < c1; ++items[4].c) {
                                                auto result = Knapsack(items, max_w);
                                                auto slow_result = SlowKnapsack(items, max_w);

                                                int res_w = 0, slow_w = 0;
                                                int res_c = 0, slow_c = 0;
                                                for (auto e: result) {
                                                    res_w += items[e].w;
                                                    res_c += items[e].c;
                                                }
                                                for (auto e: slow_result) {
                                                    slow_w += items[e].w;
                                                    slow_c += items[e].c;
                                                }
                                                total++;
                                                if (res_c != slow_c) {
                                                    incorrect++;
                                                    //continue;

                                                    std::cout << "rc sc " << res_c << ' ' << slow_c << '\n';
                                                    std::cout << max_w << '\n';
                                                    for (auto e: items) {
                                                        std::cout << e.w << ' ' << e.c << ", ";
                                                    }
                                                    std::cout << '\n';

                                                    return;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    std::cout << "it: " << incorrect << " " << total;
}

void Debug() {
    auto result = Knapsack({{4,  6},
                            {3,  1},
                            {5,  4},
                            {8,  7},
                            {9,  6},
                            {11, 0}}, 13);
    for (int elem: result) {
        std::cout << elem << ' ';
    }
}

void Solve() {
    int n, max_w;
    std::cin >> n >> max_w;

    std::vector<Item> items;
    items.reserve(n);

    for (int i = 0; i < n; ++i) {
        int w, c;
        std::cin >> w >> c;
        items.push_back({w, c});
    }

    std::vector<int> result = Knapsack(items, max_w);
    std::cout << result.size() << '\n';
    for (int id: result) {
        std::cout << id + 1 << ' ';
    }
}

int main() {
//    Test();
//    Debug();
    Solve();

    return 0;
}
