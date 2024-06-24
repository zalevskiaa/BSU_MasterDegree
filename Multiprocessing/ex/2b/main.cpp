

#include <mpi.h>

#include <cstring>

int main (int argc, char **argv)
{
    int source = 0;
    int dest = 0;
    int tag = 55;
    char message[100];
    int myrank = 0;
    int p = 0;
    MPI_Init ( &argc, &argv );
    MPI_Comm_size ( MPI_COMM_WORLD, &p );
    MPI_Comm_rank ( MPI_COMM_WORLD, &myrank );
    if ( myrank != 0 ) {
        dest = 0;
        sprintf ( message, "Hello from process %d", myrank );
        MPI_Send ( message, std::strlen(message)+1, MPI_CHAR, dest, tag, MPI_COMM_WORLD );
    }
    else {
        for ( source = 1; source < p; source++ ) {
            MPI_Recv ( message, 100, MPI_CHAR, source, tag, MPI_COMM_WORLD,
                       MPI_STATUS_IGNORE );
            printf ( "%s\n", message );
        }
    }
    MPI_Finalize ();
    return 0;
}