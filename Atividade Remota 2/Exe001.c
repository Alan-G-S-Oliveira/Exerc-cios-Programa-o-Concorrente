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
    
    int threads = omp_get_num_procs() / 2;
    int *vetor1 = gerar_vetor_inteiro(TAMANHO);
    int *vetor2 = gerar_vetor_inteiro(TAMANHO);
    int escalar, maior;
    double inicio, fim;
    double tempoSequencial, tempoParalelo;

    printf("Digite um valor: ");
    scanf("%d", &escalar);

    inicio = omp_get_wtime();
    for(int i = 0; i < TAMANHO; i++)
        vetor1[i] *= escalar;

    maior = vetor1[0];
    for(int i = 0; i < TAMANHO; i++){

        if(vetor1[i] > maior)
            maior = vetor1[i];

    }
    fim = omp_get_wtime();

    tempoSequencial = fim - inicio;

    printf("\nExecução sequencial:\n");
    printf("Maior número do vetor: %d.\n", maior);
    printf("Tempo de execução: %lf.\n\n", tempoSequencial);

    inicio = omp_get_wtime();
    #pragma omp parallel num_threads(threads)
    {

        #pragma omp for
            for(int i = 0; i < TAMANHO; i++)
                vetor2[i] *= escalar;

        
        int ID = omp_get_thread_num();
        int maiorLocal = vetor2[ID];
        int cont = 0;

        for(int i = ID; i < TAMANHO; i += threads){

            if(maiorLocal < vetor2[i])
                maiorLocal = vetor2[i];

        }

        #pragma omp critical
        {

            if(cont == 0){

                maior = maiorLocal;
                cont = 1;

            } else{

                if(maior < maiorLocal)
                    maior = maiorLocal;

            }

        }

    }
    fim = omp_get_wtime();

    tempoParalelo = fim - inicio;

    printf("Tempo de execução paralela com %d threads:\n", threads);
    printf("Maior número do vetor: %d.\n", maior);
    printf("Tempo de execução: %lf.\n", tempoParalelo);
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