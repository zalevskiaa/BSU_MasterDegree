// Задача 24. Период

// Имя входного файла: input.txt
// Имя выходного файла: output.txt
// Ограничение по времени: 1 с
// Ограничение по памяти: нет

// Для каждого префикса данной строки S, состоящей из n символов (все символы имеют ASCII код между 97 и 126 включительно), 
// необходимо узнать, является ли он периодичной строкой. 
// Это означает, что для всех i (2 ≤ i ≤ n) необходимо узнать максимальное k, 
// для которого префикс длины i строки S может быть записан как Tk, т. е. некоторая строка T, сцепленная k раз.

// Формат входных данных
// В первой строке содержится единственное число n (2 ≤ n ≤ 1 000 000) — длина строки S. Во второй строке содержится строка S.

// Формат выходных данных
// Для каждого префикса длины i, имеющего период длины i / k для k > 1, 
// необходимо вывести длину i префикса и число k повторений периода, разделяя их одним пробелом. 
// Если существует несколько таких префиксов, то выводить необходимо в порядке увеличения длины префикса.

// Примеры
// input.txt	output.txt

// 3            2 2
// aaa          3 3

// aabaabaabaab 2 2
//              6 2   
//              9 13       
//              12 4


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

int main() {
    std::ifstream fin("input.txt");
    std::ofstream fout("output.txt");
    
	int n;
	fin >> n;

    std::string s;
    fin >> s;

	std::vector<int> pi = prefix_function(s);

    for (int i = 0; i < s.length(); ++i) {
		int prefix_size = i + 1;
		int border = pi[i];
		int period = prefix_size - border;

		if (prefix_size % period) {
			continue;
		}
		int k = prefix_size / period;
		if (k < 2) {
			continue;
		}

		// std::cout << prefix_size << ' ' << border << ' ' << period << ' ' << k << '\n';
		fout << prefix_size << ' ' << k << '\n' << std::endl;
    }
}

