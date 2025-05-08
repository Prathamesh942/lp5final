#include <iostream>
#include <chrono>
#include <omp.h>

#define N 5  // Array size
#define RUNS 100000  // Number of repetitions for accurate timing

using namespace std;
using namespace std::chrono;

void linear_regression_sequential(float* x, float* y, int n, float& w, float& b) {
    float sum_x = 0, sum_y = 0, sum_xy = 0, sum_xx = 0;

    for (int i = 0; i < n; ++i) {
        sum_x += x[i];
        sum_y += y[i];
        sum_xy += x[i] * y[i];
        sum_xx += x[i] * x[i];
    }

    w = (n * sum_xy - sum_x * sum_y) / (n * sum_xx - sum_x * sum_x);
    b = (sum_y - w * sum_x) / n;
}

void linear_regression_openmp(float* x, float* y, int n, float& w, float& b) {
    float sum_x = 0, sum_y = 0, sum_xy = 0, sum_xx = 0;

    #pragma omp parallel for reduction(+:sum_x, sum_y, sum_xy, sum_xx)
    for (int i = 0; i < n; ++i) {
        sum_x += x[i];
        sum_y += y[i];
        sum_xy += x[i] * y[i];
        sum_xx += x[i] * x[i];
    }

    w = (n * sum_xy - sum_x * sum_y) / (n * sum_xx - sum_x * sum_x);
    b = (sum_y - w * sum_x) / n;
}

int main() {
    float x[N] = {1, 2, 3, 4, 5};
    float y[N] = {2.1, 4.1, 6.1, 8.1, 10.1}; // Close to y = 2x + 0.1

    float w_seq = 0, b_seq = 0, w_omp = 0, b_omp = 0;

    // Measure sequential time
    auto start_seq = high_resolution_clock::now();
    for (int i = 0; i < RUNS; ++i)
        linear_regression_sequential(x, y, N, w_seq, b_seq);
    auto end_seq = high_resolution_clock::now();
    auto duration_seq = duration_cast<nanoseconds>(end_seq - start_seq).count();

    // Measure OpenMP time
    auto start_omp = high_resolution_clock::now();
    for (int i = 0; i < RUNS; ++i)
        linear_regression_openmp(x, y, N, w_omp, b_omp);
    auto end_omp = high_resolution_clock::now();
    auto duration_omp = duration_cast<nanoseconds>(end_omp - start_omp).count();

    double avg_seq_time = static_cast<double>(duration_seq) / RUNS;
    double avg_omp_time = static_cast<double>(duration_omp) / RUNS;

    cout << fixed;
    cout << "Average Time (Sequential): " << avg_seq_time << " nanoseconds" << endl;
    cout << "Average Time (OpenMP):     " << avg_omp_time << " nanoseconds" << endl;

    if (avg_omp_time > 0)
        cout << "Speedup:                   " << avg_seq_time / avg_omp_time << "x" << endl;
    else
        cout << "Speedup:                   N/A (too fast to measure accurately)" << endl;

    cout << "\nEquation from Sequential: y = " << w_seq << " * x + " << b_seq << endl;
    cout << "Equation from OpenMP:     y = " << w_omp << " * x + " << b_omp << endl;

    return 0;
}
