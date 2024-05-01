#include <iostream>
#include <fstream>
#include <vector>

// source: https://e-maxx.ru/algo/eratosthenes_sieve

std::vector<int> MakePrimes() {
    int n = 10100;
    std::vector<char> prime (n+1, true);
    prime[0] = prime[1] = false;
    for (int i=2; i<=n; ++i)
        if (prime[i])
            if (i * 1ll * i <= n)
                for (int j=i*i; j<=n; j+=i)
                    prime[j] = false;

    std::vector<int> primes;
    primes.reserve(1240 + 1);

    for (int i = 0; i <= n; ++i) {
        if (prime[i]) {
            primes.push_back(i);
        }
    }

    return primes;
};

std::vector<int> primes = MakePrimes();

// source: https://e-maxx.ru/algo/primitive_root

int Powmod(int a, int b, int p) {
    int res = 1;
    while (b) {
        if (b & 1) {
            res = int(res * 1ll * a % p), --b;
        } else {
            a = int(a * 1ll * a % p), b >>= 1;
        }
    }
    return res;
}

int Generator(int p) {
    if (p == 2) {
        return 1;
    }

    std::vector<int> fact;
    int phi = p - 1, n = phi;

    // facrorization O(sqrt n)
//    for (int i = 2; i * i <= n; ++i) {
//        if (n % i == 0) {
//            fact.push_back(i);
//            while (n % i == 0) {
//                n /= i;
//            }
//        }
//    }
    // factorization using sieve of Eratosthenes
    for (int i = 0; i < primes.size(); ++i) {
        int prime = primes[i];
        if (prime * prime > n) {
            break;
        }
        if (n % prime == 0) {
            fact.push_back(prime);
            while (n % prime == 0) {
                n /= prime;
            }
        }
    }
    if (n > 1) {
        fact.push_back(n);
    }

    for (int res = 2; res <= p; ++res) {
        int random_res = rand() % (p - 2) + 2; // for quick passing a large test

        bool ok = true;

        for (size_t i = 0; i < fact.size() && ok; ++i) {
//            ok &= Powmod(res, phi / fact[i], p) != 1;
            ok &= Powmod(random_res, phi / fact[i], p) != 1;
        }
        if (ok) {
//            return res;
            return random_res;
        }
    }
    return -1;
}

void Debug() {

}

void Run() {
    std::ifstream fin("primes.in");
    std::ofstream fout("roots.out");

    int n;
    fin >> n;

    for (int i = 0; i < n; i++) {
        int p, g;

        fin >> p;
        g = Generator(p);
        fout << g << '\n';
    }
}

int main() {
    srand(time(0));
//    Debug();
     Run();
}