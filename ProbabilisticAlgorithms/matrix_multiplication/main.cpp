#include <iostream>
#include <bitset>
#include <vector>
#include <string>

#include <cstdlib>
#include <ctime>

#include <fstream>

int Hex2Int(char c) {
    if ('0' <= c && c <= '9') {
        return static_cast<int>(c) - '0';
    }
    return static_cast<int>(c) - 'A' + 10;
}

class Vector : public std::bitset<4000> {
};

class Matrix : public std::vector<Vector> {
public:
    Matrix(int n) : size_(n), std::vector<Vector>(n) {
    }
    void Read(std::istream& in) {
//        int block_size = size_ / 4 + static_cast<int>(size_ % 4 > 0);
        for (int row = 0; row < size_; ++row) {
            std::string row_hex;
            in >> row_hex;

            int column = 0;
            for (char e: row_hex) {
                int digit_int = Hex2Int(e);
                for (int mask: {8, 4, 2, 1}) {
                    if (digit_int & mask) {
                        (*this)[row].set(column);
                    }
                    ++column;
                    if (column >= size_) {
                        break;
                    }
                }
            }
        }
    }
    void Print() const {
        for (int i = 0; i < size_; ++i) {
            for (int j = 0; j < size_; ++j) {
                std::cout << (*this)[i][j] << ' ';
            }
            std::cout << '\n';
        }
        std::cout << '\n';
    }
    Vector operator * (const Vector& b) const {
        Vector result;
        for (int row = 0; row < size_; ++row) {
            result.set(row, ((*this)[row] & b).count() % 2 > 0);
        }
        return result;
    }
private:
    int size_;
};

Vector RandomVector(int n) {
    Vector result;
    for (int i = 0; i < n; ++i) {
        result.set(i, rand() % 2);
    }
    return result;
}

bool Solve(int n, const Matrix& a, const Matrix& b, const Matrix& c) {
    for (int i = 0; i < 50; ++i) {
        Vector x = RandomVector(n);
        if (a * (b * x) != c * x) {
            return false;
        }
    }
    return true;
}

int main() {
    srand(time(nullptr));

    std::ifstream fin("matrix.in");
    std::ofstream fout("matrix.out");

    int n;
    fin >> n;

    Matrix a(n), b(n), c(n);
    a.Read(fin);
    b.Read(fin);
    c.Read(fin);
//    a.Print();
//    b.Print();
//    c.Print();

    fout << (Solve(n, a, b, c) ? "YES" : "NO");
}
