#include <iostream>
#include <vector>
#include <algorithm>

class RandGenerator {
private:
    uint32_t cur_, a_, b_;  // беззнаковое 32-битное число
    uint32_t NextRand24() {
        cur_ = cur_ * a_ + b_;  // вычисляется с переполнениями
        return cur_ >> 8;       // число от 0 до 2**24-1.
    }

public:
    RandGenerator(uint32_t a, uint32_t b) {
        cur_ = 0;
        a_ = a;
        b_ = b;
    }
    uint32_t NextRand32() {
        uint32_t a = NextRand24(), b = NextRand24();
        return (a << 8) ^ b;  // число от 0 до 2**32-1.
    }
};

std::vector<uint32_t> MakeElems(uint32_t n, uint32_t a, uint32_t b){
    RandGenerator generator(a, b);

    std::vector<uint32_t> result;
    result.reserve(n);

    for (uint32_t i = 0; i < n; ++i) {
        uint32_t val = generator.NextRand32();
        result.push_back(val);
    }

    return result;
}

uint32_t NthElement(const std::vector<uint32_t>& r_elems, uint32_t number) {
    std::vector<uint32_t> elems = r_elems;
    std::vector<uint32_t> elems_less, elems_equal, elems_greater;
    elems_less.reserve(elems.size());
    elems_greater.reserve(elems.size());

    while (elems.size() > 1) {
        elems_less.clear();
        elems_equal.clear();
        elems_greater.clear();

        uint32_t pivot = elems[elems.size() / 2];

        for (uint32_t i = 0; i < elems.size(); ++i) {
            if (elems[i] < pivot) {
                elems_less.push_back(elems[i]);
            } else if (elems[i] > pivot) {
                elems_greater.push_back(elems[i]);
            } else {
                elems_equal.push_back(elems[i]);
            }
        }

        if (number < elems_less.size()) {
            elems = elems_less;
        } else if (number < elems_less.size() + elems_equal.size()) {
            return pivot;
        } else {
            elems = elems_greater;
            number -= elems_less.size() + elems_equal.size();
        }
    }

    return elems[0];
}

int main() {
    uint32_t n, a, b;
    std::cin >> n >> a >> b;

    std::vector<uint32_t> elems = MakeElems(n, a, b);
    uint32_t home = NthElement(elems, n / 2);

    uint64_t sum = 0;
    for (uint32_t i = 0; i < n; ++i) {
        if (home > elems[i]) {
            sum += home - elems[i];
        } else {
            sum += elems[i] - home;
        }
    }

    std::cout << sum;

    return 0;
}
