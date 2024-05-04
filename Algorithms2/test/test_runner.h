#pragma once

#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <chrono>

#include <string>
#include <vector>
#include <set>
#include <map>

template<class T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &s) {
    os << "{ ";
    bool first = true;
    for (const auto &x: s) {
        if (!first) {
            os << " ";
        }
        first = false;
        os << x;
    }
    return os << " }";
}

template<class T>
std::ostream &operator<<(std::ostream &os, const std::set<T> &s) {
    os << "{";
    bool first = true;
    for (const auto &x: s) {
        if (!first) {
            os << " ";
        }
        first = false;
        os << x;
    }
    return os << "}";
}

template<class K, class V>
std::ostream &operator<<(std::ostream &os, const std::map<K, V> &m) {
    os << "{";
    bool first = true;
    for (const auto &kv: m) {
        if (!first) {
            os << " ";
        }
        first = false;
        os << kv.first << ": " << kv.second;
    }
    return os << "}";
}

class AssertionError {
public:
    explicit AssertionError(const std::string& what_arg) {
        message_ = what_arg;
    }
    std::string What() const {
        return message_;
    }
private:
    std::string message_;
};

template<class T, class U>
void AssertEqual(const T &actual, const U &expected, const std::string &hint = {}) {
    if (!(actual == expected)) {
        std::ostringstream os;
        os << "Assertion failed!" << " Actual: " << actual << " Expected: " << expected;
        if (!hint.empty()) {
            os << " hint: " << hint;
        }
        throw AssertionError(os.str());
    }
}

inline void Assert(bool b, const std::string &hint) {
    AssertEqual(b, true, hint);
}

class Timer {
private:
    std::chrono::steady_clock::time_point begin_;
    std::chrono::steady_clock::time_point end_;

public:
    Timer() {
    }

    void Start() {
        begin_ = std::chrono::steady_clock::now();
    }

    void Stop() {
        end_ = std::chrono::steady_clock::now();
    }

    int Get() {
        return std::chrono::duration_cast<std::chrono::milliseconds>(end_ - begin_).count();
    }
};

class TestRunner {
public:
    template<class TestInfo>
    void RunTest(const TestInfo &test_info, int case_index) {
        try {
            timer_.Start();
            AssertEqual(test_info.Run(test_info.cases_[case_index].input), test_info.cases_[case_index].output);
            timer_.Stop();

            ++passed_count_;
            std::cerr << "test " << case_index << "\t OK, time: " << std::setw(4) << timer_.Get() << "ms" << std::endl;
        }
        catch (AssertionError &e) {
            timer_.Stop();

            ++failed_count_;
            std::cerr << "test " << case_index << "\t WA:, time:  " << std::setw(4) << timer_.Get() << "ms\n" << e.What() << std::endl;
        }
        catch (std::exception &e) {
            timer_.Stop();

            ++failed_count_;
            std::cerr << "test " << case_index << "\t RE: " << e.what() << std::endl;
        }
        catch (...) {
            ++failed_count_;
            std::cerr << "Unknown exception caught" << std::endl;
        }
    }

    template<class TestInfo>
    void RunTests(const TestInfo &test_info) {
        std::cerr << "---------- Testing " << test_info.name << " ---------" << std::endl;
        for (int case_index = 0; case_index < test_info.cases_.size(); ++case_index) {
            RunTest(test_info, case_index);
        }
    }

    ~TestRunner() {
        std::cerr << "---------- Test Result ---------" << std::endl;
        if (failed_count_ == 0) {
            std::cerr << passed_count_ << " tests passed" << std::endl;
        } else {
            std::cerr << failed_count_ << " tests failed of " << passed_count_ + failed_count_ << ". Terminate" << std::endl;
            exit(1);
        }
    }

private:
    Timer timer_;
    int passed_count_ = 0;
    int failed_count_ = 0;
};

//todo: expected/actual print format (when test failed)
//todo: modify TestRunner to be able printing OK / WA / RTE

//todo: memory limit lol
//todo: maybe we can ignore 'int main()' in 'main.cpp' (:

