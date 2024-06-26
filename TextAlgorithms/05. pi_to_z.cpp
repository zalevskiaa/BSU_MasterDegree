// Задача pi->z
    
// Имя входного файла: стандартный ввод
// Имя выходного файла: стандартный вывод
// Ограничение по времени: 3 с
// Ограничение по памяти: 256 МБ
        
// Загадана какая-то строка $\alpha$. 
// Вам про неё известна только длина $n$ и значения префикс-функций во всех точках. 
// Необходимо посчитать значения $z$-функции во всех точках.

// Формат входных данных
// В первой строке дана длина загаданной строки — число $n$ $(n > 0)$.
// Во второй строке дано $n$ чисел — значения префикс-функций. 
// Гарантируется, что существует как минимум одна строка, что у неё именно такая префикс-последовательность.

// Формат выходных данных
// Выведите $n$ чисел — $z$-последовательность.
// Пример
// стандартный ввод    стандартный вывод

// 7
// 0 0 1 0 1 2 3       0 0 1 0 3 0 1 


#include <iostream>
#include <vector>

std::vector<int> Pi2Z(const std::vector<int>& pi) {
    std::vector<int> z(pi.size());

    for(int i = 1; i < pi.size(); i++) {
        if(pi[i]) {
            z[i - pi[i] + 1] = pi[i];
        }
    }
    
    z[0] = pi.size();
        
    if(z[1]) {
        for(int i = 1; i < z[1]; i++) {
            z[i + 1] = z[1] - i;
        }
    }
    int t;
    for(int i = z[1] + 1; i < pi.size() - 1; i++)
    {
        t = i;
        if(z[i] && !z[i + 1]) {
            for(int j = 1; j < z[i] && z[i + j] <= z[j]; j++){
                z[i + j] = std::min(z[j], z[i] - j);
                t = i + j;
            }
        }
                
        i = t;  
    }
    
    z[0] = 0;
    return z;
}

int main() {
    int n;
    std::cin >> n;
    std::vector<int> pi(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> pi[i];
    }
    for (int e : Pi2Z(pi)) {
        std::cout << e << ' ';
    }
    std::cout << '\n';

}