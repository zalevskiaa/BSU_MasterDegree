#include <mpi.h>

#include <iostream>
#include <string>
#include <vector>

void Process(int myrank) {
    std::string self_message = "Hello from process " + std::to_string(myrank);
    MPI_Send(self_message.c_str(), self_message.length() + 1, MPI_CHAR, 0, 1, MPI_COMM_WORLD);
}

void Process0(int p) {
    std::vector<std::string > received_messages;

    for (int i = 0; i < p - 1; ++i) {
        MPI_Status status;
        int message_length = 0;

        MPI_Probe(MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_CHAR, &message_length);

        char* message = new char[message_length];
        MPI_Recv(message, message_length, MPI_CHAR, status.MPI_SOURCE, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        received_messages.push_back(message);
        delete[] message;
    }

    for (int i = 0; i < received_messages.size(); i++) {
        std::cout << received_messages[i] << "\n";
    }
}

int main(int argc, char** argv) {
    int p = 0;
    int myrank = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    if (myrank == 0) {
        Process0(p);
    } else {
        Process(myrank);
    }

    MPI_Finalize();
    return 0;
}