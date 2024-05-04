#include <iostream>
#include <vector>
#include <random>

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
    for (auto m : monotones) {
        std::cout << m.begin << ' ' << m.end << ' ' << m.type << std::endl;
    }
    //*/

    std::vector<int> result;
    result.reserve(sequence.size());
    result.push_back(sequence[0]);

    for (size_t m_index = 0; m_index < monotones.size(); ++m_index) {
        // we must add one number of every Monotone
        const Monotone& curr_m = monotones[m_index];

        if (curr_m.type == MonotoneType::increasing) {
            // pick most left elem but greater than at next_m most right
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
            // pick most right. It's greater than previous taken and any at next_m

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

bool AreAlternating(int left, int middle, int right) {
    if (left < middle && middle > right) {
        return true;
    }
    if (left > middle && middle < right) {
        return true;
    }
    return false;
}

// Returns a given sequence excluding elements which are equal to their left neighbours
// For example: { 1 1 1 2 2 3 1 } -> { 1 2 3 1 }
std::vector<int> LocalUnique(const std::vector<int> &sequence) {
    std::vector<int> result;
    if (sequence.empty()) {
        return result;
    }
    result.push_back(sequence.front());
    for (const int elem : sequence) {
        if (elem != result.back()) {
            result.push_back(elem);
        }
    }
    return result;
}

// Returns indices of sequence, where elements are
// bigger or smaller than both of their neighbours.
// For example: { 1 4 2 3 5 8 6 7 9 10 } -> { 0 1 2 5 6 9 },
// which are indices of elements: { 1 4 2 8 6 10 }.
std::vector<size_t> LocalExtremes(const std::vector<int> &sequence) {
    std::vector<size_t> result;

    result.push_back(0);
    for (size_t index = 0; index < sequence.size() - 2; ++index) {
        if (AreAlternating(sequence[index], sequence[index + 1], sequence[index + 2])) {
            result.push_back(index + 1);
        }
    }
    result.push_back(sequence.size() - 1);

    return result;
}

std::vector<int> SolveOld(const std::vector<int> &sequence) {
    std::vector<int> sequence_lu = LocalUnique(sequence);
    if (sequence_lu.size() < 2) {
        return sequence_lu;
    }

    std::vector<int> result(1, sequence_lu[0]);
    std::vector<size_t> extremes_indices = LocalExtremes(sequence_lu);

    size_t seq_index = 1;
    for (size_t ext_index = 0; ext_index < extremes_indices.size() - 2; ++ext_index) {
        while (sequence_lu[extremes_indices[ext_index]] <
                   sequence_lu[extremes_indices[ext_index + 1]] &&
               seq_index < sequence_lu.size() &&
               (sequence_lu[seq_index] < sequence_lu[extremes_indices[ext_index + 2]] ||
                sequence_lu[seq_index] < result.back())) {
            ++seq_index;
        }
        while (sequence_lu[extremes_indices[ext_index]] >
                   sequence_lu[extremes_indices[ext_index + 1]] &&
               seq_index < sequence_lu.size() &&
               (sequence_lu[seq_index] > sequence_lu[extremes_indices[ext_index + 2]] ||
                sequence_lu[seq_index] > result.back())) {
            ++seq_index;
        }
        result.push_back(sequence_lu[seq_index]);
        seq_index = extremes_indices[ext_index + 1];
        ++seq_index;
    }
    result.push_back(sequence_lu[seq_index]);

    return result;
}

std::vector<int> RandVector(size_t size) {
    std::random_device rd;   // non-deterministic generator
    std::mt19937 gen(rd());

    std::vector<int> result;
    result.reserve(size);
    for (size_t i = 0; i < size; i++) {
        result.push_back(gen() % 10 - 0 * 5);
    }
    return result;
}

bool Equal(const std::vector<int>& first, const std::vector<int>& second) {
    if (first.size() != second.size()) {
        return false;
    }
    for (size_t i = 0; i < first.size(); i++) {
        if (first[i] != second[i]) {
            return false;
        }
    }
    return true;
}

void Print(const std::vector<int>& v) {
    for (size_t i = 0; i < v.size(); i++) {
        std::cout << v[i] << " ";
    }
    std::cout << std::endl;
}

int main() {

    for (size_t t = 0; t < 1000; t++) {
        std::vector<int> v = RandVector(4);
        std::vector<int> r1 = Solve(LocalUnique(v));
        std::vector<int> r2 = SolveOld(v);
        if (!Equal(r1, r2)) {
            std::cout << "in:  ";
            Print(v);
            std::cout << "new: ";
            Print(r1);
            std::cout << "old: ";
            Print(r2);
            std::cout << std::endl;
        }
    }

    return 0;
}
