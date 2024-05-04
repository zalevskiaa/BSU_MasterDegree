#include <iostream>
#include <vector>

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
    for (int index = 0; index < sequence.size() - 2; ++index) {
        if (AreAlternating(sequence[index], sequence[index + 1], sequence[index + 2])) {
            result.push_back(index + 1);
        }
    }
    result.push_back(sequence.size() - 1);

    return result;
}

std::vector<int> Solve(const std::vector<int> &sequence) {
    std::vector<int> sequence_lu = LocalUnique(sequence);
    if (sequence_lu.size() < 2) {
        return sequence_lu;
    }

    std::vector<int> result(1, sequence_lu[0]);
    std::vector<size_t> extremes_indices = LocalExtremes(sequence_lu);

    int seq_index = 1;
    for (int ext_index = 0; ext_index < extremes_indices.size() - 2; ++ext_index) {
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

int main() {
    std::vector<int> sequence;
    int n;
    std::cin >> n;
    sequence.reserve(n);

    for (int i = 0, elem; i < n; ++i) {
        std::cin >> elem;
        sequence.push_back(elem);
    }

    for (const int elem : Solve(sequence)) {
        std::cout << elem << ' ';
    }
    std::cout << '\n';

    return 0;
}