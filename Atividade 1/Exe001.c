#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
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

    setlocale(LC_ALL, "Portuguese");
    time_t t;
    srand(time(NULL));

    int *vetor = NULL;
    int i, busca, soma = 0, quantidade = 0;
    double inicio, fim;

    vetor = gerar_vetor_inteiro(TAMANHO);

    printf("Digite o número para buscar: ");
    scanf("%d", &busca);

    inicio = omp_get_wtime();

    #pragma omp parallel num_threads(4)
    {

        #pragma omp for reduction(+: quantidade)

            for(i = 0; i < TAMANHO; i++){

                if(vetor[i] == busca)
                    quantidade++;

            }

    }

    fim = omp_get_wtime();

    printf("O número %d foi encontrado %d vezes no vetor.\n", busca, quantidade);
    printf("Tempo de execução: %.2f segundos.\n\n", fim - inicio);
    
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