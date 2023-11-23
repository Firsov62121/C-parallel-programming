#include <stdio.h>
#include <mpi.h>
#include <string.h>
//message possing interface
//MPICH - free MPI realization
//https://www.mpich.org/documentation/guides/
//https://www.mpi-forum.org/docs/mpi-3.1/mpi31-report.pdf
int main(int argc, char **argv)
{
    char buf[80];
    int rank, size, i, type = 99;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if(rank == 0)
    {
        strcpy(buf, "Hello from main!");
        for(i = 0; i < size; ++i)
            MPI_Send(buf, 80, MPI_CHAR, i, type, MPI_COMM_WORLD);
    }
    else
    {
        MPI_Status status;
        MPI_Recv(buf, 80, MPI_CHAR, 0, type, MPI_COMM_WORLD, &status);
        printf("string = %s, from process = %d\n", buf, rank);
    }
    MPI_Finalize();
    return 0;
}