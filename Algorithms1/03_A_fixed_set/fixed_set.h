//
// Created by Asus on 28.11.2021.
//

#ifndef ZALEVSKIAA_FIXED_SET_H
#define ZALEVSKIAA_FIXED_SET_H

#include <vector>
#include <list>

const size_t kSize = 400'000;

class FixedSet {
private:
    std::vector<std::vector<int>> table_;

    static size_t Hash(int value) {
        return (value * 37 + 13) % kSize;
    }

public:
    FixedSet() {}
    void Initialize(const std::vector<int>& numbers) {
        table_.clear();
        table_.resize(kSize);

        for (int number : numbers) {
            size_t index = Hash(number);

            bool not_found = true;
            for (size_t i = 0; i < table_[index].size(); ++i) {
                if (table_[index][i] == number) {
                    not_found = false;
                }
            }

            if (not_found) {
                table_[index].push_back(number);
            }
        }
    }
    bool Contains(int number) const {
        size_t index = Hash(number);
        for (size_t i = 0; i < table_[index].size(); ++i) {
            if (table_[index][i] == number) {
                return true;
            }
        }
        return false;
    }
};

#endif  // ZALEVSKIAA_FIXED_SET_H
