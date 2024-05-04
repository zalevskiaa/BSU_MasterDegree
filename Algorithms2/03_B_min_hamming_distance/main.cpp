#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

#include <cassert>
#include <iomanip>

void AssertBounds(int i_index, int j_index, int k_index, int size_1, int size_2, int size_3) {
    assert(i_index >= 0);
    assert(j_index >= 0);
    assert(k_index >= 0);
    assert(i_index < size_1);
    assert(j_index < size_2);
    assert(k_index < size_3);
}

int Solve(const std::string first, const std::string second, int k_val) {
    const int inf = 10500;
    const int size_1 = static_cast<int>(first.length()) + 1;
    const int size_2 = 2 * k_val + 1;
    const int size_3 = k_val + 1;

    if (abs(static_cast<int>(first.length()) - static_cast<int>(second.length())) > k_val) {
        return -1;
    }

    /*
    std::vector<std::vector<std::vector<int>>> result(
        size_1, std::vector<std::vector<int>>(size_2, std::vector<int>(size_3, inf)));
    */
    // this crashes on destructor for some reason :(
    // so let's use the array

    int result[size_1][size_2][size_3];
    for (int i = 0; i < size_1; ++i) {
        for (int j = 0; j < size_2; ++j) {
            for (int k = 0; k < size_3; ++k) {
                result[i][j][k] = inf;
            }
        }
    }

    for (int i_index = 0; i_index < size_1; ++i_index) {
        for (int j_index = std::max(-k_val, -i_index);
             j_index < std::min(k_val, static_cast<int>(second.length()) - i_index) + 1;
             ++j_index) {
            for (int k_index = 0; k_index < k_val + 1; ++k_index) {
                AssertBounds(i_index, j_index + k_val, k_index, size_1, size_2, size_3);
                if (i_index == 0) {
                    if (j_index <= k_index) {
                        result[i_index][j_index + k_val][k_index] = 0;
                    } else {
                        result[i_index][j_index + k_val][k_index] = inf;
                    }
                } else if (i_index + j_index == 0) {
                    if (i_index <= k_index) {
                        result[i_index][j_index + k_val][k_index] = 0;
                    } else {
                        result[i_index][j_index + k_val][k_index] = inf;
                    }
                } else {
                    result[i_index][j_index + k_val][k_index] = inf;
                    int replace = (first[i_index - 1] == second[i_index + j_index - 1]) ? 0 : 1;

                    if (k_index >= replace) {
                        AssertBounds(i_index - 1, j_index + k_val, k_index - replace, size_1, size_2,
                                     size_3);
                        result[i_index][j_index + k_val][k_index] =
                            result[i_index - 1][j_index + k_val][k_index - replace];
                    }
                    if (k_index > 0) {
                        if (j_index != k_val) {
                            AssertBounds(i_index - 1, j_index + k_val + 1, k_index - 1, size_1, size_2,
                                         size_3);
                            result[i_index][j_index + k_val][k_index] =
                                std::min(result[i_index][j_index + k_val][k_index],
                                         result[i_index - 1][j_index + k_val + 1][k_index - 1]);
                        }
                        if (j_index != -k_val) {
                            AssertBounds(i_index, j_index + k_val, k_index, size_1, size_2, size_3);
                            AssertBounds(i_index, j_index + k_val - 1, k_index - 1, size_1, size_2,
                                         size_3);
                            result[i_index][j_index + k_val][k_index] =
                                std::min(result[i_index][j_index + k_val][k_index],
                                         result[i_index][j_index + k_val - 1][k_index - 1]);
                        }
                    }
                    if (first[i_index - 1] != second[i_index + j_index - 1]) {
                        AssertBounds(i_index - 1, j_index + k_val, k_index, size_1, size_2, size_3);
                        result[i_index][j_index + k_val][k_index] =
                            std::min(result[i_index][j_index + k_val][k_index],
                                     result[i_index - 1][j_index + k_val][k_index] + 1);
                    }
                }
            }
        }
        continue;

        std::cout << i_index << '\n';
        for (int j_index = 0; j_index < size_2; ++j_index) {
            for (int k_index = 0; k_index < size_3; ++k_index) {
                if (result[i_index][j_index][k_index] < inf) {
                    std::cout << std::setw(2) << result[i_index][j_index][k_index] << ' ';
                } else {
                    std::cout << std::setw(2) << "." << ' ';
                }
            }
            std::cout << '\n';
        }
        std::cout << '\n';
    }

    int ans = inf;
    for (int k = 0; k < k_val + 1; ++k) {
        ans = std::min(ans, result[static_cast<int>(first.length())][static_cast<int>(second.length()) -
                                                                     static_cast<int>(first.length()) + k_val][k]);
    }
    return ans;
}

//*

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string first = "aa";
    std::string second = "a";
    int k_val = 1;

    std::cin >> first >> second >> k_val;
    std::cout << Solve(first, second, k_val);

    std::cout << std::flush;
    return 0;
}

//*/
