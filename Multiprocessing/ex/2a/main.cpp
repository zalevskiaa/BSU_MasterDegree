#include <mpi.h>
int main (int argc, char **argv)
{
    int myrank = 0;
    int p = 0;
    MPI_Init ( &argc, &argv );
    MPI_Comm_size ( MPI_COMM_WORLD, &p );
    MPI_Comm_rank ( MPI_COMM_WORLD, &myrank );
    printf ( "Hello from process %d (communicator size: %d)\n", myrank, p );
    MPI_Finalize ();
    return 0;
}