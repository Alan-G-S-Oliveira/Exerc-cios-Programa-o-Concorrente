#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>
#include <omp.h>

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
    setlocale(LC_ALL, "Portuguese");
    time_t t;
    srand(time(NULL));
    int *vetor = NULL;
    int threads = omp_get_num_procs() / 2;
    int maior, indiceMaior, cont = 0;
    double inicio, fim;
    double tempoSequencial, tempoParalelo;

    vetor = gerar_vetor_inteiro(TAMANHO);
    
    printf("A execução paralela é feita com %d threads.\n\n", threads);

    maior = vetor[0];
    indiceMaior = 0;

    inicio = omp_get_wtime();
    for(int i = 1; i < TAMANHO; i++){

        if(vetor[i] > maior){

            maior = vetor[i];
            indiceMaior = i;

        } 

    }
    fim = omp_get_wtime();

    tempoSequencial = fim - inicio;

    printf("Execução sequencial:\n");
    printf("O maior elemento do vetor é: %d. Localizado no índice %d.\n", maior, indiceMaior);
    printf("Tempo de execução: %lf.\n\n", tempoSequencial);

    maior = vetor[0];
    indiceMaior = 0;

    inicio = omp_get_wtime();
    #pragma omp parallel num_threads(threads)
    {

        int Maior, IndiceMaior;
        int ID = omp_get_thread_num();

        Maior = maior;
        IndiceMaior = indiceMaior;

        for(int i = ID; i < TAMANHO; i += threads){

            if(vetor[i] > Maior){

                Maior = vetor[i];
                IndiceMaior = i;

            }

        }

        #pragma omp critical
        {

            if(cont == 0){

                maior = Maior;
                indiceMaior = IndiceMaior;

                cont = 1;

            } else if(Maior >= maior){

                maior = Maior;

                if(IndiceMaior < indiceMaior)
                    indiceMaior = IndiceMaior;

            }

        }

    }
    fim = omp_get_wtime();

    tempoParalelo = fim - inicio;

    printf("Execução paralela com o omp critical:\n");
    printf("O maior elemento do vetor é: %d. Localizado no índice: %d.\n", maior, indiceMaior);
    printf("Tempo de execução: %lf.\n", tempoParalelo);
    printf("Speed up: %lf.\n", tempoSequencial / tempoParalelo);
    printf("Eficiência: %lf.\n\n", (tempoSequencial / tempoParalelo) / threads);
    
    
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