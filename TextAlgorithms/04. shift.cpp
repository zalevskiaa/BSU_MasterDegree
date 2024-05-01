// Задача 0.1. Циклический сдвиг

// Имя входного файла: input.txt
// Имя выходного файла: output.txt
// Ограничение по времени: 3 с
// Ограничение по памяти: нет


// Циклическим сдвигом строки на k символов будем называть строку, которая получается из исходной строки переносом первых k символов в конец строки. Вам необходимо определить, можно ли получить из одной строки другую при помощи некоторого циклического сдвига.

// Формат входных данных
// В первой строке записано единственное число n (1 ≤ n ≤ 1 000 000). Далее следуют две строки по n маленьких латинских букв.

// Формат выходных данных
// Выведите единственное число — минимальную величину сдвига (число k), который переводит первую строку во вторую; если преобразование невозможно, то вывести число −1.

// Примеры
// input.txt	output.txt
// 10          1
// aaaaaaaaab
// aaaaaaaaba

// 10          2
// aaaaaaaaab
// aaaaaaabaa

// 7           -1
// aabaabc
// baabaaa


#include <iostream>
#include <fstream>
#include <vector>
#include <string>

std::vector<int> prefix_function (std::string s) {
	int n = (int) s.length();
	std::vector<int> pi (n);
	for (int i=1; i<n; ++i) {
		int j = pi[i-1];
		while (j > 0 && s[i] != s[j])
			j = pi[j-1];
		if (s[i] == s[j])  ++j;
		pi[i] = j;
	}
	return pi;
}

int SInT(const std::string& str, const std::string& text) {
    // returns first occurancy of str in text or -1

    std::vector<int> pi = prefix_function(str + '#' + text);
    for (int i = str.length() + 1; i < pi.size(); ++i) {
        if (pi[i] == str.length()) {
            return i - 2 * str.length();
        }
    }
    return -1;
}

int main() {
    std::ifstream fin("input.txt");
    std::ofstream fout("output.txt");
    

    int n;
    std::string a;
    std::string b;

    fin >> n >> a >> b;

    fout << SInT(b, a + a) << '\n';
    
    
}