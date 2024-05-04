#include <iostream>
#include <deque>


int main() {
    std::deque<int> d;
    d.push_back(0);
    d.push_back(1);
    d.push_back(2);
    d.push_back(3);
    for (int x: d) {
        std::cout << x << " ";
    }
    std::cout << '\n';

    std::cout << d[0] << d[1] << "\n";

    d.push_front(-11);
    d.push_back(44);

    for (int x: d) {
        std::cout << x << " ";
    }
    std::cout << '\n';

    std::cout << d[0] << d[1] << "\n";


}
