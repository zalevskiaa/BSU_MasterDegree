#include <iostream>

#include <vector>
#include <string>
#include <cmath>

// source: https://gist.github.com/hwei/1950649d523afd03285c
static unsigned int Hash(const std::string& str)
{
    uint32_t fnv_prime = 16777619;
    uint32_t offset_basis = 2166136261;

    unsigned int hash = offset_basis;
    for (size_t i = 0; i < str.length(); ++i)
    {
        hash ^= str[i];
//        hash *= fnv_prime;
        hash += (hash << 1) + (hash << 4) + (hash << 8) + (hash << 23);
    }
    return hash;
}

void AddTestUsers(std::vector<uint8_t>& values, uint32_t inv_k) {
    for (int i = 0; i < 22999; i++) {
        uint32_t hash_val = Hash("id" + std::to_string(i));
        uint32_t index = hash_val >> inv_k;
        uint32_t value = hash_val & ((1 << inv_k) - 1);

        uint16_t first_nonzero_bit = inv_k - static_cast<int>(log2(value));

        values[index] = std::max(static_cast<uint16_t>(values[index]), first_nonzero_bit);
    }
}

// source: https://algo.inria.fr/flajolet/Publications/FlFuGaMe07.pdf
int main() {
    uint32_t k = 18;
    uint32_t m = pow(2, k);

    double a = 0.7213 / (1 + 1.079 / m);

    uint32_t inv_k = 32 - k;

    std::vector<uint8_t> values(m, 0);
    std::string s;

    while (getline(std::cin, s)) {
        if (s.empty()) {
            break;
        }
        uint32_t hash_val = Hash(s);
        uint32_t index = hash_val >> inv_k;
        uint32_t value = hash_val & ((1 << inv_k) - 1);

        uint16_t first_nonzero_bit = inv_k - static_cast<int>(log2(value));

        values[index] = std::max(static_cast<uint16_t>(values[index]), first_nonzero_bit);
    }

//    AddTestUsers(values, inv_k);

    double estimate = 0;
    uint32_t empty_cells = 0;

    for (int i = 0; i < values.size(); ++i) {
        estimate += pow(2, -values[i]);
        if (values[i] == 0) {
            empty_cells++;
        }
    }
    estimate = (1.0 / estimate) * a * m * m;

    if ((estimate < (2.5 * m)) && (empty_cells > 0)) {
        estimate = m * log(static_cast<double>(m) / empty_cells);
    }
    if (estimate > (1.0 / 30) * UINT32_MAX) {
        estimate = -(UINT32_MAX * (1.0 - estimate / UINT32_MAX));
    }

    std::cout << static_cast<int>(estimate) << std::endl;

    return 0;
}