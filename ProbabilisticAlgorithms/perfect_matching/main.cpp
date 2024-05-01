
// C++ program to find the inverse of Matrix.

#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <vector>


const int kP = 1'000'000'007;
//const int kP = 7;

int OpUnaryMinus(int x) {
    return kP - x;
}

int OpAdd(int a, int b) {
    return (a + b) % kP;
}

int OpSub(int a, int b) {
    return (a - b + kP) % kP;
}

int OpMul(int a, int b) {
    return static_cast<int64_t>(a) * b % kP;
}

int OpPow(int x, int p) {
    if (p == 0) {
        return 1;
    }
    if (p & 1) {
        return OpMul(x, OpPow(x, p - 1));
    }
    int y = OpPow(x, p / 2);
    return OpMul(y, y);
}

int OpInverse(int x) {
    return OpPow(x, kP - 2);
}

int OpDiv(int a, int b) {
    if (b == 0) {
        // todo: Деление на ноль в Гауссе возникает, в этих случаях возвращаем all false
        // todo: Но мб есть случаи, когда можно свапнуть строки и таки найти обратную матрицу
        throw "lol";
    }
    return OpMul(a, OpInverse(b));
}


void PrintMatrix(int** ar, int n, int m)
{
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            std::cout << ar[i][j] << "  ";
        }
        printf("\n");
    }
}

void InverseOfMatrix(int** matrix, int n)
{
    PrintMatrix(matrix, n, n);

    for (int i = 0; i < n; i++) {
        matrix[i][n + i] = 1;
    }

    for (int i = n - 1; i > 0; i--) {
        if (matrix[i - 1][0] < matrix[i][0]) {
            std::swap(matrix[i], matrix[i - 1]);
        }
    }

    PrintMatrix(matrix, n, n * 2);

    // Replace a row by sum of itself and a
    // constant multiple of another row of the matrix
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (j != i) {
                int temp = OpDiv(matrix[j][i], matrix[i][i]);
                for (int k = 0; k < 2 * n; k++) {
                    matrix[j][k] = OpSub(matrix[j][k], OpMul(matrix[i][k], temp));
                }
            }
        }
    }

    for (int i = 0; i < n; i++) {
        int temp = matrix[i][i];
        for (int j = 0; j < 2 * n; j++) {
            matrix[i][j] = OpDiv(matrix[i][j], temp);
        }
    }

//    PrintMatrix(matrix, n, n * 2);
}

void Debug() {
    std::cout << RAND_MAX;
}

std::vector<bool> Solve(int n, const std::vector<std::pair<int, int>>& edges) {
    if (n & 1) {
        return std::vector<bool> (edges.size(), false);
    }

    int** matrix = new int*[n];
    for (int i = 0; i < n; i++) {
        matrix[i] = new int[2 * n];
        for (int j = 0; j < 2 * n; ++j) {
            matrix[i][j] = 0;
        }
    }
    for (const auto& edge: edges) {
        matrix[edge.first][edge.second] = rand() % kP;
        matrix[edge.second][edge.first] = rand() % kP;
    }

    try {
        InverseOfMatrix(matrix, n);
    } catch (...) {
        return std::vector<bool> (edges.size(), false);
    }

    std::vector<bool> result;
    result.reserve(edges.size());

    for (const auto& edge: edges) {
        result.push_back(matrix[edge.first][n + edge.second] != 0 || matrix[edge.second][n + edge.first] != 0);
    }

    return result;
}

int main()
{
//    Debug();
//    return 0;

    std::ifstream fin("matching.in");
    std::ofstream fout("matching.out");

    int n, m;
    fin >> n >> m;

    std::vector<std::pair<int, int>> edges;
    edges.reserve(m);

    for (int i = 0; i < m; ++i) {
        int a, b;
        fin >> a >> b;
        if (a > b) {
            std::swap(a, b);
        }
        edges.push_back({a - 1, b - 1});
    }

    auto result = Solve(n, edges);
    for (const auto elem: result) {
        fout << (elem ? "YES" : "NO") << "\n";
    }

    return 0;
}