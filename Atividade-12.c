#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <mpi.h>

#define TAMANHO 10000000
#define MAX 10

int main() {

    srand(time(NULL));

    MPI_Init(NULL, NULL);
    int nprocs, rank;

    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int i;
    long soma_total = 0;
    int quantidades = TAMANHO / 2;
    int *vetor = NULL;
    int *subvetor = NULL;

    double media;

    if(rank == 0) {

        vetor = (int *)malloc(sizeof(int) * TAMANHO);

        for(int i = 0; i < TAMANHO; i++)
            vetor[i] = rand() % (MAX + 1);

    }

    subvetor = (int *)malloc(sizeof(int) * quantidades);
    MPI_Scatter(vetor, quantidades, MPI_INT, subvetor, quantidades, MPI_INT, 0, MPI_COMM_WORLD);

    long soma = 0;
    for(i = 0; i < quantidades; i++)
        soma += subvetor[i];

    long *somas = NULL;
    if(rank == 0)
        somas = (long *)malloc(sizeof(long) * nprocs);

    MPI_Gather(&soma, 1, MPI_LONG, somas, 1, MPI_LONG, 0, MPI_COMM_WORLD);

    if(rank == 0) {

        for(i = 0; i < nprocs; i++)
            soma_total += somas[i];

        media = soma_total / (double) TAMANHO;
        printf("Media: %lf.\n", media);

    }

    double aux;
    MPI_Scatter(&media, 1, MPI_DOUBLE, &aux, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    double diferencaQuadrados = 0;
    for(i = 0; i < quantidades; i++)
        diferencaQuadrados += (aux - subvetor[i]) * (aux - subvetor[i]);

    double *diferencas = NULL;
    if(rank == 0)
        diferencas = (double *)malloc(sizeof(double) * nprocs);

    MPI_Gather(&diferencaQuadrados, 1, MPI_DOUBLE, diferencas, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    double diferencaTotal = 0;
    if(rank == 0) {

        for(i = 0; i < nprocs; i++)
            diferencaTotal += diferencas[i];

        double desvioPadrao = sqrt(diferencaTotal / TAMANHO);

        printf("Desvio padrao: %lf\n\n", desvioPadrao);

    }

    free(somas);
    free(vetor);
    free(subvetor);
    free(diferencas);

    MPI_Finalize();

    return 0;
}