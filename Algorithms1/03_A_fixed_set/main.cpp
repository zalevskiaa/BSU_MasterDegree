#include <iostream>
#include <vector>
#include <stdexcept>
#include <cstring>

int rand() {  // NOLINT
    throw std::runtime_error("Don't use rand");
}

#include "fixed_set.h"

std::vector<int> ReadSequence() {
    size_t size;
    std::cin >> size;
    std::vector<int> sequence(size);
    for (auto& current : sequence) {
        std::cin >> current;
    }
    return sequence;
}

std::vector<bool> PerformRequests(const std::vector<int>& requests, const FixedSet& set) {
    std::vector<bool> request_answers;
    request_answers.reserve(requests.size());
    for (int request: requests) {
        request_answers.push_back(set.Contains(request));
    }
    return request_answers;
}

void PrintRequestsResponse(const std::vector<bool>& request_answers) {
    for (bool answer: request_answers) {
        std::cout << (answer ? "Yes" : "No") << "\n";
    }
}

void RunTests();

int main(int argc, char **argv) {
    if (argc > 1 && !strcmp(argv[1], "--testing")) {
        RunTests();
        return 0;
    }

    std::ios::sync_with_stdio(false);

    auto numbers = ReadSequence();
    auto requests = ReadSequence();
    FixedSet set;
    set.Initialize(numbers);
    PrintRequestsResponse(PerformRequests(requests, set));

    return 0;
}

// ========= TESTING ZONE =========

#define ASSERT_EQ(expected, actual) do { \
    auto __expected = expected; \
    auto __actual = actual; \
    if (!(__expected == __actual)) { \
        std::cerr << __FILE__ << ":" << __LINE__ << ": Assertion error" << std::endl; \
        std::cerr << "\texpected: " << __expected << " (= " << #expected << ")" << std::endl; \
        std::cerr << "\tgot: " << __actual << " (= " << #actual << ")" << std::endl; \
        std::cerr << "=========== FAIL ===========\n"; \
        throw std::runtime_error("Check failed"); \
    } \
} while (false)

void Empty() {
    FixedSet set;
    set.Initialize({});
    ASSERT_EQ(false, set.Contains(0));
}

void Simple() {
    FixedSet set;
    set.Initialize({-3, 5, 0, 3, 7, 1});
    ASSERT_EQ(true, set.Contains(0));
    ASSERT_EQ(true, set.Contains(-3));
    ASSERT_EQ(true, set.Contains(1));
    ASSERT_EQ(false, set.Contains(2));
    ASSERT_EQ(false, set.Contains(4));
}

void RepeatInitialize() {
    FixedSet set;
    const int shift = 100;
    int element = 0;
    int last = -1;
    for (int elements_count = 0; elements_count < 10; ++elements_count) {
        std::vector<int> elements;
        for (int i = 0; i < elements_count; ++i) {
            elements.push_back(element++);
        }
        set.Initialize(elements);
        for (auto elem : elements) {
            ASSERT_EQ(true, set.Contains(elem));
        }
        ASSERT_EQ(false, set.Contains(last));
        last = element - 1;
        element += shift;
    }
}

void Magic() {
#ifdef MAGIC
    std::cerr << "You've been visited by Hash Police!\n";
    std::cerr << "Probably your hash table is not as good as you think.\n";
    std::cerr << "No ticket today, but you better be careful.\n\n";
    int first = -1'000'000'000;
    int second = first + MAGIC;
    FixedSet set;
    set.Initialize({first, second});
    ASSERT_EQ(true, set.Contains(first));
    ASSERT_EQ(true, set.Contains(second));
    ASSERT_EQ(false, set.Contains(0));
#endif
}

void RunTests() {
    std::cerr << "Running tests...\n";
    Empty();
    Simple();
    RepeatInitialize();
    Magic();
    std::cerr << "Tests are passed!\n";
}