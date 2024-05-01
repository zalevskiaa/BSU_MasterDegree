#include <iostream>
// #include <cstdlib>
#include <chrono>
#include <iomanip>

#include <immintrin.h>

#include<functional>


inline double fRand01() {
    return static_cast<double>(rand()) / RAND_MAX;
}

void fill_arrays(double* a, double* b, int n) {
    for (int i = 0; i < n; ++i) {
        a[i] = fRand01();
        b[i] = fRand01();
    }
}

double* make_array(int n) {
    return new double[n];
}

void delete_array(double*& a, int n) {
    delete[] a;
    a = nullptr;
}

double func(const double* a, const double* b, int n) {
    double sum = 0;

    for (int i = 0; i < n; ++i) {
        sum += a[i] / b[i];
    }

    return sum;
}

double func_p(const double* a, const double* b, int n) {
    __m256d sum_pd = _mm256_setzero_pd();
    
    int shift = 0;
    for (; shift + 3 < n; shift += 4) {
        __m256d a_pd = _mm256_loadu_pd(a + shift);
        __m256d b_pd = _mm256_loadu_pd(b + shift);

        __m256d a_div_b_pd = _mm256_div_pd(a_pd, b_pd);

        sum_pd = _mm256_add_pd(sum_pd, a_div_b_pd);
    }

    double sum_buffer[4];
    _mm256_storeu_pd(sum_buffer, sum_pd);

    double result = sum_buffer[0] + sum_buffer[1] + sum_buffer[2] + sum_buffer[3];
    for (; shift < n; ++shift) {
        result += a[shift] / b[shift];
    }

    return result;
}

double func_p_prefetch(const double* a, const double* b, int n) {
    __m256d sum_pd = _mm256_setzero_pd();
    
    constexpr int PREFETCH_SIZE = 16; 

    int shift = 0;
    for (; shift + 3 < n; shift += 4) {
        if (shift % PREFETCH_SIZE == 0) {
            _mm_prefetch(a + shift, PREFETCH_SIZE);
            _mm_prefetch(b + shift, PREFETCH_SIZE);
        }
        __m256d a_pd = _mm256_loadu_pd(a + shift);
        __m256d b_pd = _mm256_loadu_pd(b + shift);

        __m256d a_div_b_pd = _mm256_div_pd(a_pd, b_pd);

        sum_pd = _mm256_add_pd(sum_pd, a_div_b_pd);
    }

    double sum_buffer[4];
    _mm256_storeu_pd(sum_buffer, sum_pd);

    double result = sum_buffer[0] + sum_buffer[1] + sum_buffer[2] + sum_buffer[3];
    for (; shift < n; ++shift) {
        result += a[shift] / b[shift];
    }

    return result;
}

double run_one(const double* a, const double* b, int n, std::function<void(const double*, const double*, int)> func) {
    auto t1 = std::chrono::steady_clock::now();

    func(a, b, n);

    auto t2 = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> ms_double = t2 - t1;

    return ms_double.count();
}

double run(int n, std::function<void(const double*, const double*, int)> func, bool refill_arrays) {
    double* a = make_array(n);
    double* b = make_array(n);

    constexpr int WARMUP_COUNT = 10;
    constexpr int RUNS_COUNT = 100;
    
    double ms_sum = 0;

    fill_arrays(a, b, n);

    for (int i = 0; i < WARMUP_COUNT; ++i) {
        run_one(a, b, n, func);
    }
    for (int i = 0; i < RUNS_COUNT; ++i) {
        if (refill_arrays) {
            fill_arrays(a, b, n);
        }

        double ms_double = run_one(a, b, n, func);
        std::cout << "i: " << i << ", time: " << std::setprecision(4) << ms_double << " ms\n";

        ms_sum += ms_double;
    }

    delete_array(a, n);
    delete_array(b, n);

    double ms_mean = ms_sum / RUNS_COUNT;
    return ms_mean;
}

const double* debug_func(const double* a, const double* b) {
    __m256i mask_256_pi = _mm256_setr_epi64x(-1, -1, -1, -1);

    __m256d a_pd = _mm256_maskload_pd(a, mask_256_pi);
    __m256d b_pd = _mm256_maskload_pd(b, mask_256_pi);
    
    __m256d sum_pd = _mm256_add_pd(a_pd, b_pd);

    double* sum = new double[4];
    _mm256_maskstore_pd(sum, mask_256_pi, sum_pd);

    return sum;
}

void debug_dunc_2() {
    // Mask-load 256-bit double-precision vector
    double doubles_256[4] = {5.0, 6.0, 7.0, 8.0};
    
    __m256i mask_256_pd = _mm256_setr_epi64x(-1, -1, -1, -1);

    __m256d double_vec_256 = _mm256_maskload_pd(doubles_256, mask_256_pd);

    double* dou_256 = (double*) &double_vec_256;
    printf("double:\t\t%lf, %lf, %lf, %lf\n", dou_256[0], dou_256[1], dou_256[2], dou_256[3]);
}

void debug() {
    // debug_dunc_2();

    // return;


    double* a = new double[4];
    a[0] = 1.07;
    a[1] = 12;
    a[2] = 13;
    a[3] = 14;
    

    double* b = new double[4];
    b[0] = 1;
    b[1] = 1111.05;
    b[2] = 1222;
    b[3] = 1333;

    const double* res = debug_func(a, b);

    for (int i = 0; i < 4; ++i) {
        std::cout << res[i] << ' ';
    }
    std::cout << '\n';
}

void test() {
    double* a = new double[4];
    a[0] = 0.5;
    a[1] = 1;
    a[2] = 0;
    a[3] = 1.001230321;

    double* b = new double[4];
    b[0] = 1;
    b[1] = 2321;
    b[2] = 1;
    b[3] = 0.2321;
    
    int n = 4;

    // double expected = func(a, b, n);
    // double actual = func_p(a, b, n);

    // std::cout << expected << ' ' << actual << '\n';
}

int main() {
    double ms_double = run(100'000'000, func, true);
    std::cout << ms_double << " ms\n";

    // debug();
    // test();

    return 0;
}