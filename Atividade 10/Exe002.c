#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

int main() {

    int rank, nprocs;
    char mensagem[20];

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if(rank == 0){

        strcpy(mensagem, "Hello World!");
        MPI_Bcast(mensagem, 20, MPI_CHAR, 0, MPI_COMM_WORLD);

    } else {

        MPI_Recv(mensagem, 20, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Mensagem %s recebida pelo processo %d.\n", mensagem, rank);

    }

    MPI_Finalize();

    return 0;
}
