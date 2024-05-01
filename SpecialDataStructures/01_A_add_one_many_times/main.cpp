#include <iostream>
#include <vector>
#include <numeric>

const int kBinMaxSize = 62;
std::vector<int64_t> two_pow;

std::vector<int64_t> MakeTwoPow(int size) {
    std::vector<int64_t> result(size, 1);
    for (int i = 1; i < result.size(); ++i) {
        result[i] = 2 * result[i - 1];
    }
    return result;
}

int64_t BinToDec(const std::vector<int>& bin) {
    int64_t result = 0;
    for (int i = 0; i < bin.size(); ++i) {
        result += bin[i] * two_pow[i];
    }
    return result;
}

std::vector<int> DecToBin(int64_t dec) {
    std::vector<int> result;
    result.reserve(kBinMaxSize);
    while (dec > 0) {
        result.push_back(dec % 2);
        dec /= 2;
    }
    return result;
}

int HighestDifferentBit(const std::vector<int>& a, const std::vector<int>& b) {
    if (a.size() != b.size()) {
        if (a.size() > b.size()) {
            throw "idk";
        }
        return b.size();
    }
    for (int i = a.size() - 1; i >= 0; --i) {
        if (a[i] != b[i]) {
            if (a[i] == 1 && b[i] == 0) {
                throw "idk";
            }
            return i;
        }
    }
    throw "idk";
}

int64_t NumberBitChanges(int64_t dec) {
    int64_t result = 0;
    for (int i = 0; two_pow[i] <= dec; ++i) {
        result += dec / two_pow[i];
    }
    return result;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    two_pow = MakeTwoPow(kBinMaxSize);

    // input

    int a_bin_size;
    std::cin >> a_bin_size;
    std::vector<int> a_bin(a_bin_size, 0);
    for (int i = 0; i < a_bin_size; ++i) {
        std::cin >> a_bin[a_bin_size - i - 1];
    }
    int64_t n;
    std::cin >> n;


    int64_t a_dec = BinToDec(a_bin);
    int64_t b_dec = a_dec + n;
    std::vector<int> b_bin = DecToBin(b_dec);

    // removing leading zeros, making a and b equal-sized

    while (a_bin.size() > b_bin.size() && a_bin.back() == 0) {
        a_bin.pop_back();
    }
    if (a_bin.size() != b_bin.size()) {
        throw "lol";
    }

    // max

    int hdf = HighestDifferentBit(a_bin, b_bin);
    std::cout << hdf + 1 << '\n';

    // mean

    int64_t numerator = NumberBitChanges(b_dec) - NumberBitChanges(a_dec);
    int64_t denominator = n;
    int64_t gcd = std::gcd(numerator, denominator);

    std::cout << numerator / gcd << '/' << denominator / gcd << '\n';

    std::cout << std::flush;
    return 0;
}
