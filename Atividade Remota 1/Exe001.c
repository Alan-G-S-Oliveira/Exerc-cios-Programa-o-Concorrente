#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAX 100
#define TAMANHO 100000000

double *gerar_vetor(int n);
int *gerar_vetor_inteiro(int n);
void mostrar_vetor_inteiro(int *v,int tamanho);
void mostrar_vetor(double *v,int tamanho);

int main() {
    
    int threads = omp_get_num_procs() / 2;

    double *vetor = gerar_vetor(TAMANHO);
    double media, soma = 0;
    double desvioPadrao, somaQuadrados = 0;
    double inicio, fim;
    double tempoSequencial, tempoParalelo;

    inicio = omp_get_wtime();
    for(int i = 0; i < TAMANHO; i++)
        soma += vetor[i];

    media = soma / TAMANHO;

    for(int i = 0; i < TAMANHO; i++)
        somaQuadrados += (vetor[i] - media) * (vetor[i] - media);

    desvioPadrao = sqrt(somaQuadrados / TAMANHO);
    fim = omp_get_wtime();

    tempoSequencial = fim - inicio;

    printf("Execução sequencial:\n");
    printf("Desvio padrão: %lf.\n", desvioPadrao);
    printf("Tempo de execução: %lf.\n\n", tempoSequencial);

    soma = 0;
    somaQuadrados = 0;

    inicio = omp_get_wtime();
    #pragma omp parallel num_threads(threads)
    {

        int ID = omp_get_thread_num();
        double somar = 0;

        for(int i = ID; i < TAMANHO; i += threads)
            somar += vetor[i];

        #pragma omp critical
            soma += somar;

    }
    media = soma / TAMANHO;
    
    #pragma omp parallel num_threads(threads)
    {

        int ID = omp_get_thread_num();
        double somar = 0;

        for(int i = ID; i < TAMANHO; i += threads)
            somar += (vetor[i] - media) * (vetor[i] - media);

        #pragma omp critical
            somaQuadrados += somar;

    }
    desvioPadrao = sqrt(somaQuadrados / TAMANHO);
    fim = omp_get_wtime();

    tempoParalelo = fim - inicio;

    printf("Execução paralela com %d threads:\n", threads);
    printf("Desvio padrão: %lf.\n", desvioPadrao);
    printf("Tempo de execução: %lf.\n", fim - inicio);
    printf("Speed up: %lf.\n", tempoSequencial / tempoParalelo);
    printf("Eficiência: %lf.\n\n", (tempoSequencial / tempoParalelo) / threads);

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