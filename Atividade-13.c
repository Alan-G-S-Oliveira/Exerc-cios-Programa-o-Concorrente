#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#define TAMANHO 10

int *gerar_vetor_binario(int n);

int main() {

  srand(time(NULL));

  MPI_Init(NULL, NULL);

  int rank, nprocs;

  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
  MPI_Comm_size(MPI_COMM_WORLD, &rank);

  int *vetor = NULL;
  int *subvetor = NULL;

  int quantidades = TAMANHO / nprocs;
  int quantidades_uns = 0;
  int uns_totais = 0;

  if(rank == 0){

    vetor = (int *)malloc(sizeof(int) * TAMANHO);

    for(int i = 0; i < TAMANHO; i++)
      vetor[i] = rand() % 2;

  }

  subvetor = (int *)malloc(sizeof(int) * quantidades);
  MPI_Scatter(vetor, quantidades, MPI_INT, subvetor, quantidades, MPI_INT, 0, MPI_COMM_WORLD);

  if(rank == 0) {

        vetor = (int *)malloc(sizeof(int) * TAMANHO);

        for(int i = 0; i < TAMANHO; i++)
            vetor[i] = rand() % (MAX + 1);

    }

    subvetor = (int *)malloc(sizeof(int) * quantidades);
    MPI_Scatter(vetor, quantidades, MPI_INT, subvetor, quantidades, MPI_INT, 0, MPI_COMM_WORLD);

  for(int i = 0; i < quantidades; i++) {

    if(subvetor[i] == 1)
      quantidades_uns++;

  }

  int *uns = NULL;
  if(rank == 0)
    uns = (int *)malloc(sizeof(int) * nprocs);

  MPI_Gather(&quantidades_uns, 1, MPI_INT, uns, 1, MPI_INT, 0, MPI_COMM_WORLD);

  if(rank == 0) {

    for(int i = 0; i < nprocs; i++)
      uns_totais += uns[i];

    if(uns_totais % 2 != 0) {

      vetor = (int *)realloc(vetor, sizeof(int) * (TAMANHO + 1));
      vetor[TAMANHO] = 1;

      uns_totais++;

    }

    printf("Total de uns: %d.\n\n", uns_totais);

  }

  free(uns);
  free(vetor);
  free(subvetor);

  MPI_Finalize();

  return 0;
}

int *gerar_vetor_binario(int n) {

  srand(time(NULL));

  int *vetor;
  int i; 
  vetor = (int *)malloc(sizeof(int) * n);
  
  for (i=0;i<n;i++) {
    int num = rand() % 2;
    vetor[i] = num;
  }
  return vetor;
}