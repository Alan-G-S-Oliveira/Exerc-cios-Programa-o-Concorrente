#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main() {

    int nprocs, rank;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if(rank == 0){

        int x;
        
        printf("Digite um valor: ");
        scanf("%d", &x);

        MPI_Send(&x, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);

    } else {

        int y;

        MPI_Recv(&y, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        printf("O valor %d foi recebido pelo processo %d.\n\n", y, rank);

    }

    MPI_Finalize();

    return 0;
}