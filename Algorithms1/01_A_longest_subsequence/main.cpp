#include <iostream>
#include <vector>

enum MonotoneType { increasing, decreasing };

MonotoneType GetMonotoneType(int first, int second) {
    return first < second ? MonotoneType::increasing : MonotoneType::decreasing;
}

MonotoneType Inverted(MonotoneType monotone_type) {
    if (monotone_type == MonotoneType::increasing) {
        return MonotoneType::decreasing;
    } else {
        return MonotoneType::increasing;
    }
}

struct Monotone {
    size_t begin, end;
    MonotoneType type;
};

std::vector<int> Solve(const std::vector<int>& sequence) {
    if (sequence.size() < 2) {
        return sequence;
    }
    std::vector<Monotone> monotones;
    monotones.reserve(sequence.size());

    monotones.push_back(Monotone({1, 2, GetMonotoneType(sequence[0], sequence[1])}));
    while (monotones.back().end < sequence.size()) {
        MonotoneType last_type = monotones.back().type;
        size_t last_index = monotones.back().end - 1;
        size_t next_index = monotones.back().end;

        if (last_type == GetMonotoneType(sequence[last_index], sequence[next_index])) {
            ++monotones.back().end;
        } else {
            monotones.push_back(Monotone({next_index, next_index + 1, Inverted(last_type)}));
        }
    }

    /*
    for (auto var_m : monotones) {
        std::cout << var_m.begin << ' ' << var_m.end << ' ' << var_m.type << std::endl;
    }
    //*/

    std::vector<int> result;
    result.reserve(sequence.size());
    result.push_back(sequence[0]);

    for (size_t m_index = 0; m_index < monotones.size(); ++m_index) {
        // we must add one number of every Monotone
        const Monotone& curr_m = monotones[m_index];

        if (curr_m.type == MonotoneType::increasing) {
            // pick bridge left elem but greater than at next_m bridge right
            size_t seq_index = curr_m.begin;

            if (m_index + 1 < monotones.size()) {
                const Monotone& next_m = monotones[m_index + 1];
                while (sequence[seq_index] <= sequence[next_m.end - 1] && seq_index < curr_m.end) {
                    ++seq_index;
                }
            }
            while (sequence[seq_index] <= result.back() && seq_index < curr_m.end) {
                ++seq_index;
            }

            result.push_back(sequence[seq_index]);

        } else {
            // pick bridge right. It's greater than previous taken and any at next_m

            size_t seq_index = curr_m.begin;

            if (m_index + 1 < monotones.size()) {
                const Monotone& next_m = monotones[m_index + 1];
                while (sequence[seq_index] >= sequence[next_m.end - 1] && seq_index < curr_m.end) {
                    ++seq_index;
                }
            }
            while (sequence[seq_index] >= result.back() && seq_index < curr_m.end) {
                ++seq_index;
            }

            result.push_back(sequence[seq_index]);
        }
    }

    return result;
}

int main() {
    size_t sequence_size;
    std::cin >> sequence_size;
    std::vector<int> sequence;
    sequence.reserve(sequence_size);

    int elem;
    std::cin >> elem;
    sequence.push_back(elem);
    for (size_t i = 1; i < sequence_size; ++i) {
        std::cin >> elem;
        if (elem != sequence.back()) {
            sequence.push_back(elem);
        }
    }

    // std::vector<int> sequence;
    // sequence = {1, 4, 2, 3, 5, 8, 6, 7, 9, 10};
    // sequence = {10, 22, 9, 33, 49, 50, 31, 60};
    // sequence = {1,2,3,4,5,6,7,5,3};
    // sequence = {1,2,4,6,3,1,5,7,8,9,10};
    // sequence = {1,2,3,4,5};
    // sequence = {5, 3, 1, 2, 3, 4, 5, 3};
    // sequence = {1,3,5,4};
    // sequence = {1,3,5,4,2,1,3,5};
    // sequence = {1,1,4,4,2,2,3,3,5,5,8,8,6,6,7,7,9,9,10,10};
    // sequence = {1};
    // sequence = {1};
    // sequence = {1};

    for (const int x : Solve(sequence)) {
        std::cout << x << ' ';
    }
    std::cout << '\n';

    return 0;
}
