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
    int n_threads = omp_get_num_procs() / 2;
    int oMaior, cont = 0;

    oMaior = vetor[0];
    for(int i = 1; i < TAMANHO; i++){

        if(vetor[i] > oMaior)
            oMaior = vetor[i];

    }

    printf("%d", oMaior);

    #pragma omp parallel num_threads(n_threads)
    {

        for(int i = 0; i < TAMANHO; i++)

    }

    #pragma omp parallel num_threads(n_threads)
    {

        int ID = omp_get_thread_num();
        int maior = vetor[ID];

        for(int i = ID; i < TAMANHO; i += n_threads){

            if(vetor[i] > maior)
                maior = vetor[i];

        }

        #pragma omp critical
        {

            if(cont == 0){

                oMaior = maior;
                cont = 1;
            
            } else{

                if(maior > oMaior)
                    oMaior = maior;

            }

        }

    }

    printf("O maior número do vetor: %d.\n\n", oMaior);

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