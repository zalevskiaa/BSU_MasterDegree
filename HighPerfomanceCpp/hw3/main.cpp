#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <atomic>


bool is_happy(int64_t x) {
    if (x < 0) {
        x = -x;
    }

    int last_digit = -1;

    while (x > 0) {
        int digit = x % 10;
        if (digit < last_digit) {
            return false;
        }
        last_digit = digit;

        x /= 10;
    }
    return true;
}

void thread_count_happy_numbers(int64_t begin, int64_t end, std::atomic_int64_t* happy_count, std::atomic_int64_t* total_count) {
    *happy_count = 0;
    *total_count = 0;

    for (int64_t i = begin; i < end; ++i) {
        if (is_happy(i)) {
            ++(*happy_count);
        }
        ++(*total_count);
    }
}

void thread_write_total_sum(const std::vector<std::atomic_int64_t>* total_counts, const std::atomic_bool* keep_writing) {
    using std::chrono::steady_clock;

    for (steady_clock::time_point start = steady_clock::now(), awake = start; *keep_writing; awake += std::chrono::seconds(5)) {
        std::this_thread::sleep_until(awake);

        int64_t total_sum = 0;
        for (int i = 0; i < total_counts->size(); ++i) {
            total_sum += total_counts->at(i);
        }

        std::chrono::duration<int64_t, std::nano> ns = awake - start;
        std::cout << "total: " << total_sum << ", time: " << ns.count() / 1'000'000 << " ms\n";
    }
}

std::pair<int64_t, int64_t> count_happy_numbers(int64_t n, int t) {
    int64_t step = n / t;

    std::vector<std::atomic_int64_t> happy_counts(t);
    std::vector<std::atomic_int64_t> total_counts(t);

    // writing thread
    std::atomic_bool keep_writing = true;
    std::thread write_thread(thread_write_total_sum, &total_counts, &keep_writing);

    // working threads
    std::vector<std::thread> threads;
    for (int i = 0; i < t; ++i) {
        int64_t begin = step * i;
        int64_t end = i < t - 1 ? begin + step : n;
        
        threads.emplace_back(thread_count_happy_numbers, begin, end, &happy_counts[i], &total_counts[i]);
    }

    // working threads join
    for (std::thread& thread : threads) {
        thread.join();
    }

    // writing thread join
    keep_writing = false;
    write_thread.join();

    std::pair<int64_t, int64_t> happy_total(0, 0);
    for (int i = 0; i < t; ++i) {
        happy_total.first += happy_counts[i];
        happy_total.second += total_counts[i];
    }

    return happy_total;
}


int main(int argc, char** argv) {
    int64_t n = 10'000'000'000;
    int t = std::stoi(argv[1]);

    std::cout << "t: " << t << "\n";

    std::pair<int64_t, int64_t> happy_total = count_happy_numbers(n, t);
    std::cout << "happy: " << happy_total.first << ", total: " << happy_total.second << '\n';
}