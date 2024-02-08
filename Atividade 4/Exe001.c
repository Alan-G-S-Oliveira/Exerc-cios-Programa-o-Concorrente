#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 100
#define TAMANHO 100000000

double *gerar_vetor(int n);
int *gerar_vetor_inteiro(int n);
void mostrar_vetor_inteiro(int *v,int tamanho);
void mostrar_vetor(double *v,int tamanho);

int main() {
    /*
    INICIO
    */
    time_t t;
    srand(time(NULL));
    int *vetor = NULL;
    vetor = gerar_vetor_inteiro(TAMANHO);
    int n_thrads = omp_get_num_procs() / 2;
    int x, soma = 0;
    double inicio, fim;

    printf("Digite um número: ");
    scanf("%d", &x);

    inicio = omp_get_wtime();
    for(int i = 0; i < TAMANHO; i++){

        if(vetor[i] == x)
            soma++;

    }
    fim = omp_get_wtime();

    printf("Existe %d valores de %d no vetor.\n", soma, x);
    printf("Tempo de execução sequencial: %f segundos.\n", fim - inicio);

    soma = 0;
    inicio = omp_get_wtime();
    #pragma omp parallel num_threads(n_thrads)
    {

        #pragma omp for reduction(+:soma)
        for(int i = 0; i < TAMANHO; i++){

            if(vetor[i] == x)
                soma++;

        }

    }
    fim = omp_get_wtime();

    printf("Tempo de execução com reduction: %lf.\n", fim - inicio);

    soma = 0;
    inicio = omp_get_wtime();
    #pragma omp parallel num_threads(n_thrads)
    {

        int ID, somar = 0;

        ID = omp_get_thread_num();

        for(int i = ID; i < TAMANHO; i += n_thrads){

            if(vetor[i] == x)
                somar++;

        }

        #pragma omp critical
            soma += somar;

    }
    fim = omp_get_wtime();

    printf("Tempo de execução com critical: %lf.\n\n", fim - inicio);

    /*
    FIM
    */
    return 0;
}

double *gerar_vetor(int n) {
    double *vetor;
    int i;
    vetor = (double *)malloc(sizeof(double) * n);
    for (i=0;i<n;i++) {
        double num = (rand() / (float)RAND_MAX);
        vetor[i] = num;
    }
    return vetor;
}

int *gerar_vetor_inteiro(int n) {
    int *vetor;
    int i;
    vetor = (int *)malloc(sizeof(int) * n);
    for (i=0;i<n;i++) {
        int num = (rand() % MAX);
        vetor[i] = num;
    }
    return vetor;
}

void mostrar_vetor(double *v,int tamanho) {
    int i;
    for (i=0;i<tamanho;i++) {
        printf("[%.5f]",v[i]);
    }
    printf("\n");
}

void mostrar_vetor_inteiro(int *v,int tamanho) {
    int i;
    for (i=0;i<tamanho;i++) {
        printf("[%d]",v[i]);
    }
    printf("\n");
}