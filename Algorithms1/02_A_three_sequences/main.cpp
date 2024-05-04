#include <iostream>
#include <vector>
#include <algorithm>

int Max(int first, int second, int third) {
    return std::max(first, std::max(second, third));
}

std::vector<int> LeastCommonSubsequence(const std::vector<std::vector<int>>& sequences) {
    size_t size_first = sequences[0].size();
    size_t size_second = sequences[1].size();
    size_t size_third = sequences[2].size();

    std::vector<std::vector<std::vector<int>>> dp(
        size_first + 1,
        std::vector<std::vector<int>>(size_second + 1, std::vector<int>(size_third + 1, 0)));
    for (size_t index_i = 1; index_i < size_first + 1; ++index_i) {
        int elem_i = sequences[0][index_i - 1];
        for (size_t index_j = 1; index_j < size_second + 1; ++index_j) {
            int elem_j = sequences[1][index_j - 1];
            for (size_t index_k = 1; index_k < size_third + 1; ++index_k) {
                int elem_k = sequences[2][index_k - 1];

                if (elem_i == elem_j && elem_j == elem_k) {
                    dp[index_i][index_j][index_k] = dp[index_i - 1][index_j - 1][index_k - 1] + 1;
                } else {
                    int max_val =
                        Max(dp[index_i - 1][index_j][index_k], dp[index_i][index_j - 1][index_k],
                            dp[index_i][index_j][index_k - 1]);

                    if (max_val == dp[index_i - 1][index_j][index_k]) {
                        dp[index_i][index_j][index_k] = dp[index_i - 1][index_j][index_k];
                    } else if (max_val == dp[index_i][index_j - 1][index_k]) {
                        dp[index_i][index_j][index_k] = dp[index_i][index_j - 1][index_k];
                    } else {
                        dp[index_i][index_j][index_k] = dp[index_i][index_j][index_k - 1];
                    }
                }
            }
        }
    }

    //    for (size_t index_i = 1; index_i < size_first + 1; ++index_i) {
    //        for (size_t index_j = 1; index_j < size_second + 1; ++index_j) {
    //            std::cout << std::setw(2) << dp[index_i][index_j] << ' ';
    //        }
    //        std::cout << '\size_first';
    //    }

    std::vector<int> result;
    size_t index_i = size_first, index_j = size_second, index_k = size_third;
    while (index_i > 0 && index_j > 0 && index_k > 0) {
        int elem_i = sequences[0][index_i - 1];
        int elem_j = sequences[1][index_j - 1];
        int elem_k = sequences[2][index_k - 1];

        if (elem_i == elem_j && elem_j == elem_k) {
            result.push_back(elem_i);
            index_i -= 1;
            index_j -= 1;
            index_k -= 1;
        } else {
            int max_val = Max(dp[index_i - 1][index_j][index_k], dp[index_i][index_j - 1][index_k],
                              dp[index_i][index_j][index_k - 1]);

            if (max_val == dp[index_i - 1][index_j][index_k]) {
                index_i -= 1;
            } else if (max_val == dp[index_i][index_j - 1][index_k]) {
                index_j -= 1;
            } else {
                index_k -= 1;
            }
        }
    }
    std::reverse(result.begin(), result.end());
    return result;
}

int main() {
    std::vector<std::vector<int>> sequences(3, std::vector<int>());
    for (size_t i = 0; i < 3; ++i) {
        size_t size;
        int value;
        std::cin >> size;
        sequences[i].reserve(size);
        for (size_t j = 0; j < size; ++j) {
            std::cin >> value;
            sequences[i].push_back(value);
        }
    }

    std::vector<int> result(LeastCommonSubsequence(sequences));

    std::cout << result.size() << '\n';

    for (size_t i = 0; i < result.size(); ++i) {
        std::cout << result[i] << ' ';
    }

    return 0;
}
