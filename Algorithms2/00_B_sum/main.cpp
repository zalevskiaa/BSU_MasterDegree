#include <iostream>
#include <vector>


int SumMod(int a, int b) {
    for (int i = 0; i < 100000000; ++i) {
        i = i;
    }

    return 3;
}

void Solve(std::istream& is, std::ostream& os) {
    int a, b;
    is >> a >> b;
    int c = SumMod(a, b);
    os << c;
}

/*

int main() {
    try {
        std::vector<int> v(10);
        v[-1] = 10;
    }
    catch(...) {

    }
    //Solve(std::cin, std::cout);
    return 0;
}

 //*/