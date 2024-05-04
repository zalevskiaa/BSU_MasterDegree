#include <iostream>
#include <string>
#include <vector>

std::vector<int> ZFunction(const std::string& s) {
    int size = static_cast<int>(s.length());
    std::vector<int> result(size);
    int l = 0, r = 0;
    for (int i = 1; i < size; ++i) {
        if (i <= r) {
            result[i] = std::min(result[i - l], r - i + 1);
        }
        while (i + result[i] < size && s[result[i]] == s[i + result[i]]) {
            ++result[i];
        }
        if (i + result[i] - 1 > r) {
            l = i, r = i + result[i] - 1;
        }
    }
    result[0] = size;
    return result;
}

void Solve(std::istream &is, std::ostream &os) {
    std::string pattern, text;
    is >> pattern >> text;
    if (pattern.length() > text.length()) {
        os << '0' << '\n';
        return;
    }

    std::string pattern_text = pattern + text;
    std::string pattern_hat_text_hat(pattern.length() + text.length(), 'a');
    for (int i = 0; i < static_cast<int>(pattern.length()); ++i) {
        pattern_hat_text_hat[i] = pattern[pattern.length() - 1 - i];
    }
    for (int i = 0; i < static_cast<int>(text.length()); ++i) {
        pattern_hat_text_hat[pattern.length() + i] = text[text.length() - 1 - i];
    }

    std::vector<int> z_func = ZFunction(pattern_text);
    std::vector<int> z_func_hat = ZFunction(pattern_hat_text_hat);

    std::vector<int> result;
    result.reserve(text.length() - pattern.length() + 1);
    for (int i = 0; i < static_cast<int>(text.length()) - static_cast<int>(pattern.length()) + 1; ++i) {
        if (z_func[pattern.length() + i] + z_func_hat[text.length() - i] + 1 >= static_cast<int>(pattern.length())) {
            result.push_back(i);
        }
    }

    os << result.size() << '\n';
    for (int i = 0; i < static_cast<int>(result.size()); ++i) {
        os << result[i] + 1 << ' ';
    }
}

/*

int main() {
    std::ios::sync_with_stdio(false);
    Solve(std::cin, std::cout);

    return 0;
}

//*/