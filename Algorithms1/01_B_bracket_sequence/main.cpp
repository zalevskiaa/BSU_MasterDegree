#include <iostream>

#include <string>
#include <stack>

bool IsLeft(char bracket) {
    return bracket == '(' || bracket == '[' || bracket == '{';
}

bool IsRight(char bracket) {
    return bracket == ')' || bracket == ']' || bracket == '}';
}

bool IsPair(char left, char right) {
    if (left == '(' && right == ')') {
        return true;
    }
    if (left == '[' && right == ']') {
        return true;
    }
    if (left == '{' && right == '}') {
        return true;
    }
    return false;
}

struct SolveResult {
    bool correct;
    size_t answer;
};

SolveResult Solve(const std::string &sequence) {
    std::stack<char> st;
    for (size_t counter = 0; counter < sequence.length(); ++counter) {
        char bracket = sequence[counter];
        if (IsRight(bracket)) {
            if (st.empty()) {
                return {false, counter};
            }
            if (IsPair(st.top(), bracket)) {
                st.pop();
                continue;
            }
            return {false, counter};
        }
        if (IsLeft(bracket)) {
            st.push(bracket);
            continue;
        }
        return {false, counter};
    }
    return {st.empty(), sequence.length()};
}

int main() {
    std::string sequence;
    std::cin >> sequence;

    SolveResult result = Solve(sequence);
    if (result.correct) {
        std::cout << "CORRECT";
    } else {
        std::cout << result.answer;
    }

    return 0;
}
