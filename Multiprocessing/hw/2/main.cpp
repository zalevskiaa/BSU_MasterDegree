#include <mpi.h>

#include <iostream>
#include <vector>
#include <cmath>

double Func(double x) {
    return x * sin(10 * x);
}

double TrapezoidalIntegral(double a, double b, int r) {
    double width = (b - a) / r;
    double result = 0;

    for (int step = 0; step < r; step++) {
        double x1 = a + step * width;
        double x2 = a + (step + 1) * width;

        result += (x2 - x1) * (Func(x1) + Func(x2)) / 2;
    }

    return result;
}

void Slave(int r) {
    double eps = 1e-8;

    double result = 0;
    double ab_received[2];

    while (true) {
        MPI_Send(&result, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
        MPI_Recv(ab_received, 2, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        if (ab_received[1] + eps < ab_received[0]) {
            // case (b < a) is interpreted as stop-signal
            break;
        }
        result = TrapezoidalIntegral(ab_received[0], ab_received[1], r);
    }
}

double MasterReceiveSend(double a, double b) {
    MPI_Status status;
    double received_result;
    double ab_send[2] = {a, b};

    MPI_Recv(&received_result, 1, MPI_DOUBLE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
    MPI_Send(ab_send, 2, MPI_DOUBLE, status.MPI_SOURCE, 0, MPI_COMM_WORLD);

    return received_result;
}

void Master(int p, double a, double b, int n) {
    double result = 0;
    double width = (b - a) / n;

    for (int step = 0; step < n; ++step) {
        result += MasterReceiveSend(a + step * width, a + (step + 1) * width);
    }
    for (int slave = 1; slave < p; ++slave) {
        result += MasterReceiveSend(1, 0);
    }

    std::cout << "Result: " << result << "\n";
}

int main() {
    double a = 0, b = 1; // integrate bounds
    int n = 1000;   // segments number
    int r = 100;    // number of each segment's segments

    MPI_Init(nullptr, nullptr);

    int p, my_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    if (my_rank == 0) {
        Master(p, a, b, n);
    } else {
        Slave(r);
    }

    MPI_Finalize();
}