#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
#include <string>

const double kPi = 3.14159265358979;

struct Complex {
    double re, im;
    Complex(double r_val = 0, double i_val = 0) {
        re = r_val;
        im = i_val;
    }
    Complex operator+(const Complex &other) const {
        return Complex(re + other.re, im + other.im);
    }
    Complex operator-(const Complex &other) const {
        return Complex(re - other.re, im - other.im);
    }
    Complex operator*(const Complex &other) const {
        return Complex(re * other.re - im * other.im, re * other.im + im * other.re);
    }
};
int CharToInt(char c) {
    switch (c) {
        case 'A':
            return 0;
        case 'C':
            return 1;
        case 'G':
            return 2;
        default:
            return 3;
    }
}

void Change(std::vector<Complex> &eps, int len) {
    for (int i = 1, j = len / 2; i < len - 1; ++i) {
        if (i < j) {
            std::swap(eps[i], eps[j]);
        }
        int k = len / 2;
        while (j >= k) {
            j -= k;
            k >>= 1;
        }
        if (j < k) {
            j += k;
        }
    }
}

void Fft(std::vector<Complex> &eps, int len, int op) {
    Change(eps, len);
    for (int h = 2; h <= len; h <<= 1) {
        Complex wn(cos(-op * 2 * kPi / h), sin(-op * 2 * kPi / h));
        for (int j = 0; j < len; j += h) {
            Complex w(1, 0);
            for (int k = j; k < j + h / 2; k++) {
                Complex u = eps[k];
                Complex t = w * eps[k + h / 2];
                eps[k] = u + t;
                eps[k + h / 2] = u - t;
                w = w * wn;
            }
        }
    }
    if (op == -1) {
        for (int i = 0; i < len; i++) {
            eps[i].re /= len;
        }
    }
}

void Conv(const std::string &text, const std::string &pattern, std::vector<Complex> &first,
          std::vector<Complex> &second, const std::vector<std::vector<int>> &can,
          std::vector<int> &sum, int len, int char_index) {

    for (int i = 0; i < static_cast<int>(text.length()); i++) {
        first[i] = Complex(can[i][char_index], 0);
    }
    for (int i = text.length(); i < len; i++) {
        first[i] = Complex(0, 0);
    }
    for (int i = 0; i < static_cast<int>(pattern.length()); i++) {
        if (CharToInt(pattern[i]) == char_index) {
            second[i] = Complex(1, 0);
        } else {
            second[i] = Complex(0, 0);
        }
    }
    for (int i = pattern.length(); i < len; i++) {
        second[i] = Complex(0, 0);
    }
    Fft(first, len, 1);
    Fft(second, len, 1);
    for (int i = 0; i < len; i++) {
        first[i] = first[i] * second[i];
    }
    Fft(first, len, -1);
    for (int i = 0; i < len; i++) {
        sum[i] += static_cast<int>(first[i].re + 0.5);
    }
}

int Solve(const std::string &text, std::string &pattern) {
    int len = 1;
    while (len < static_cast<int>(text.length()) * 2 || len < static_cast<int>(pattern.length()) * 2) {
        len <<= 1;
    }

    std::vector<std::vector<int>> can(text.length(), std::vector<int>(4, 0));
    std::vector<int> sum(std::max(len, static_cast<int>(text.length() + pattern.length() - 1)), 0);
    std::vector<Complex> first(len);
    std::vector<Complex> second(len);

    for (int i = 0; i < static_cast<int>(text.length()); i++) {
        can[i][CharToInt(text[i])] = 1;
    }

    std::reverse(pattern.begin(), pattern.end());
    for (int i = 0; i < 4; ++i) {
        Conv(text, pattern, first, second, can, sum, len, i);
    }

    int best_pos = pattern.size() - 1;

    for (int i = pattern.size() - 1; i < static_cast<int>(text.size()); ++i) {
        if (sum[i] > sum[best_pos]) {
            best_pos = i;
        }
    }

    return best_pos - (pattern.size() - 1) + 1;
}

void Release() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string text;
    std::string pattern;
    std::cin >> text >> pattern;

    std::cout << Solve(text, pattern) << std::flush;
}

//*

int main() {
    Release();

    return 0;
}

//*/
