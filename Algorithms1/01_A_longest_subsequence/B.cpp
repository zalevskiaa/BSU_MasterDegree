// C программа для поиска самой длинной чередующейся подпоследовательности в
// массив
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

#include <random>
#include <ctime>

// функция, возвращающая максимум двух чисел
int Max(int a, int b) {
    return (a > b) ? a : b;
}

void PrintVector2(const std::vector<std::vector<int>> las) {
    size_t n = las.size();
    size_t m = las[0].size();
    for (int j = 0; j < m; j++) {
        for (int i = 0; i < n; i++) {
            std::cout << las[i][j] << ' ';
        }
        std::cout << '\n';
    }
}

// Функция для возврата самой длинной длины чередующейся подпоследовательности
int Zzis(const std::vector<int>& v)
{
    /* las[i][0] = Длина самой длинной чередующейся подпоследовательности
        заканчивающийся индексом i и последний элемент больше
        чем его предыдущий элемент
        las[i][1] = длина самой длинной чередующейся подпоследовательности
        заканчивающийся индексом i и последний элемент меньше
        чем его предыдущий элемент */

    int n = v.size();
    std::vector< std::vector<int> > las(n, std::vector<int>(2, 1));

    int res = 1;  // Инициализировать результат
    /* Рассчитать значения снизу вверх */
    for (int i = 1; i < n; i++)
    {
        // Рассмотрим все элементы как предыдущие arr [i]
        for (int j = 0; j < i; j++)
        {
            // Если arr [i] больше, то проверяем с помощью las [j] [1]
            if (v[j] < v[i] && las[i][0] < las[j][1] + 1) {
                las[i][0] = las[j][1] + 1;
            }
            // Если arr [i] меньше, то проверяем с помощью las [j] [0]
            if (v[j] > v[i] && las[i][1] < las[j][0] + 1) {
                las[i][1] = las[j][0] + 1;
            }
        }
        /* Выберите максимум обоих значений по индексу i */
            if (res < Max(las[i][0], las[i][1])) {
                res = Max(las[i][0], las[i][1]);
            }
    }
    // PrintVector2(las);
    return res;
}

void Shuffle(std::vector<int>& v) {
    for (size_t i = 0; i < v.size(); ++i) {
        std::random_device rd;   // non-deterministic generator
        std::mt19937 gen(rd());
        size_t j = gen() % v.size();
        std::swap(v[i], v[j]);
    }
}

int main()
{
    std::vector<int> v = {1, 4, 2, 3, 3, 5, 8, 6, 7, 9, 10};
    // v = {10, 22, 9, 33, 49, 50, 31, 60};

    /*
    v.resize(10000);
    // std::iota(v.begin(), v.end(), 1);

    std::random_device rd;   // non-deterministic generator
    std::mt19937 gen(rd());
    for (int i = 0; i < v.size(); i++)
    v[i] = gen() % 1000000000;

    Shuffle(v);
    */

    std::cout << v.size() << std::endl;
    for (size_t i = 0; i < v.size(); i++) {
        std::cout << v[i] << " ";
    }
    std::cout << std::endl;

    size_t t1 = clock();
    std::cout << Zzis(v) << std::endl;
    size_t t2 = clock();
    std::cout << static_cast<double>(t2 - t1) / CLOCKS_PER_SEC << std::endl;

    return 0;
}