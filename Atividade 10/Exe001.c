#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define NUM_STEPS 1000000000

int main(){

    int nprocs, rank;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double step = 1 / (double)NUM_STEPS;
    double pi = 0, piTotal = 0;
    double x, soma = 0;

    for (int i = rank; i < NUM_STEPS; i += nprocs){

        x = (i + 0.5) * step;
        soma += 4 / (1 + x * x);

    }

    pi = soma * step;

    MPI_Reduce(&pi, &piTotal, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if(rank == 0)
        printf("Valor de PI: %lf.\n\n", piTotal);

    MPI_Finalize();

    return 0;
}