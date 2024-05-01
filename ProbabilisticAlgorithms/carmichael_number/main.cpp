#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>

static uint64_t pow_mod(uint64_t a, uint64_t x, uint64_t n) {
    uint64_t r = 1;

    while (x) {
        if ((x & 1) == 1)
            r = (__int128_t)a * r % n;  // Slow
                                        //            r = a*r % n;

        x >>= 1;
        a = (__int128_t)a * a % n;  // SLow
                                    //        a = a*a % n;
    }

    return r;
}

/*
 * Return an integer between a and b.
 *
 * Note that we use rand() here, meaning that all its pathological cases
 * will apply here as well --- i.e., it's slow and not very random --- but
 * should suffice.
 *
 */
int64_t RandBetween(uint64_t a, uint64_t b) {
    // todo: maybe this func is bad?
    // Assume rand_func() is 32 bits
    uint64_t r = (static_cast<uint64_t>(rand()) << 32) | static_cast<uint64_t>(rand());
    //    return a + static_cast<uint64_t>(static_cast<double>(b - a + 1) * r / (UINT64_MAX + 1.0));
    return r % (b - a + 1) + a;
}


/*
 * The Miller-Rabin probabilistic primality test.
 *
 * Returns true if ``n´´ is PROBABLY prime, false if it's composite.
 * The parameter ``k´´ is the accuracy.
 *
 * The running time should be somewhere around O(k log_3 n).
 *
 */
bool Isprime(uint64_t n, int k) {
    // Must have ODD n greater than THREE
    if (n == 2 || n == 3)
        return true;
    if (n <= 1 || !(n & 1))
        return false;

    // Write n-1 as d*2^s by factoring powers of 2 from n-1
    int s = 0;
    for (uint64_t m = n - 1; !(m & 1); ++s, m >>= 1);  // loop

    uint64_t d = (n - 1) / (uint64_t(1) << s);

    for (int i = 0; i < k; ++i) {
        //        uint64_t a = rand_between(2, n - 2);
        uint64_t a = RandBetween(3, n - 1);

        uint64_t x = pow_mod(a, d, n);

        if (x == 1 || x == n - 1)
            continue;

        for (int r = 1; r <= s - 1; ++r) {
            x = pow_mod(x, 2, n);
            if (x == 1)
                return false;
            if (x == n - 1)
                goto LOOP;
        }

        return false;
    LOOP:
        continue;
    }

    // n is *probably* prime
    return true;
}

int64_t Gcd(int64_t a, int64_t b) {
    while (b > 0) {
        a %= b;
        std::swap(a, b);
    }
    return a;
}

int64_t GetRandomCoprimeNumber(int64_t n) {
    while (true) {
        int64_t coprime = RandBetween(2, n - 1);
        if (Gcd(coprime, n) == 1) {
            return coprime;
        }
    }
}

bool Iscarmichael(uint64_t n) {
    if (n == 0 || n == 1) {
        return false;
    }
    if (Isprime(n, 100)) {
        return false;
    }
    for (int i = 0; i < 100; ++i) {
        int64_t rcn = GetRandomCoprimeNumber(n);
        if (pow_mod(rcn, n - 1, n) != 1) {
            return false;
        }
    }
    return true;
}

void Debug() {
}

void Run() {
    std::ifstream fin("carmichael.in");
    std::ofstream fout("carmichael.out");

    int n;
    fin >> n;

    for (int i = 0; i < n; i++) {
        uint64_t x;

        fin >> x;
        if (Iscarmichael(x)) {
            fout << "Yes\n";
        } else {
            fout << "No\n";
        }
    }
}

int main() {
    srand(time(0));
    //    Debug();
    Run();
}