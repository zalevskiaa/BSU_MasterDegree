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
public:
    int Compare(const Vector& b) const {
        // return -1 if equal else any index where inequal

        std::bitset<4000> result = (*this) ^ b;
        int index = result._Find_first();

        if (0 <= index && index < result.size()) {
            return index;
        }
        return -1;
    }
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

//Vector operator * (const Vector& a, const Matrix& b) {
//    Vector result;
//    for (int col = 0; col < b.size(); ++col) {
//        int sum = 0;
//        for (int row = 0; row < b.size(); ++row) {
//            sum += a[row] & b[row][col];
//        }
//        result.set(col, sum % 2 > 0);
//    }
//    return result;
//}

int FindCol(const Vector& a, const Matrix& b, const Vector& c) {
    for (int col = 0; col < b.size(); ++col) {
        int sum = 0;
        for (int row = 0; row < b.size(); ++row) {
            sum += a[row] & b[row][col];
        }

        if (c[col] != ((sum % 2) > 0)) {
            return col;
        }
    }
    throw "unreachable";
}

Vector RandomVector(int n) {
    Vector result;
    for (int i = 0; i < n; ++i) {
        result.set(i, rand() % 2);
    }
    return result;
}

std::pair<int, int> Solve(int n, const Matrix& a, const Matrix& b, const Matrix& c) {
    // (-1, -1) if correct else (r, c)

    for (int i = 0; i < 30; ++i) {
        Vector x = RandomVector(n);
        int row = (a * (b * x)).Compare(c * x);
        if (row == -1) {
            continue;
        }

//        int col = (a[row] * b).Compare(c[row]);
        int col = FindCol(a[row], b, c[row]);
        return {row, col};
    }
    return {-1, -1};
}

int main() {
    srand(time(nullptr));

    std::ifstream fin("element.in");
    std::ofstream fout("element.out");

    while (true) {
        int n;
        fin >> n;

        if (n == 0) {
            break;
        }

        Matrix a(n), b(n), c(n);
        a.Read(fin);
        b.Read(fin);
        c.Read(fin);
//        a.Print();
//        b.Print();
//        c.Print();

        // write "Yes \n i j" if incorrect else "No"
        std::pair<int, int> result = Solve(n, a, b, c);
        if (result.first == -1) {
            fout << "No\n";
        } else {
            fout << "Yes\n" << result.first + 1 << ' ' << result.second + 1 << '\n';
        }

//        std::cout << "----------\n";
    }
}
