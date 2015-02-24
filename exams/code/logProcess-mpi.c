#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

struct histogramEntry {
    int zipcode; int num;
};

int initializeLogs(int ** logs) {
    // omitted initialization code...
}

struct histogramEntry * countZips(int * logs, int start, int count) {
    struct histogramEntry * results =
        malloc(100000*sizeof(struct histogramEntry));
    for (int i = start; i < count; i++) results[logs[i]].num++;
    return results;
}

int main (int argc, char * argv[]) {
    int rank, size, count;
    int * logs;

    MPI_Init (&argc, &argv);
    MPI_Comm_rank (MPI_COMM_WORLD, &rank);
    MPI_Comm_size (MPI_COMM_WORLD, &size);
    if (rank == 0) {
        count = initializeLogs(&logs);
        for (int i = 0; i < size; i++) {
            MPI_Send(&count, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(logs, count, MPI_INT, i, 0, MPI_COMM_WORLD);
            // also, collate results returned from worker threads.
        }
    } else {
        MPI_Status stat;
        MPI_Recv(&count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &stat);
        MPI_Recv(logs, count, MPI_INT, 0, 0, MPI_COMM_WORLD, &stat);
        int local_size = count / (size-1);
        struct histogramEntry * result = 
          countZips(logs, (rank-1) * local_size, local_size);
        // also, return results to master
    }
    MPI_Finalize();
    return 0;
}

