#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <omp.h>

int main(){

    setlocale(LC_ALL, "Portuguese");

    int threads = omp_get_num_procs() / 2;
    int numero = 1000000000;
    double inicio, fim;
    double tempoSequencial, tempoParalelo;
    long int soma = 0;

    printf("A execução paralela é feita com %d threads.\n\n", threads);

    inicio = omp_get_wtime();
    for(int i = 1; i <= numero; i++){

        if(numero % i == 0)
            soma += i;

    }
    fim = omp_get_wtime();

    tempoSequencial = fim - inicio;

    printf("Execução  sequencial:\n");
    printf("A soma dos divisores de %d é %ld.\n", numero, soma);
    printf("Tempo de execução: %lf.\n\n", tempoSequencial);

    soma = 0;

    inicio = omp_get_wtime();
    #pragma omp parallel num_threads(threads)
    {

        #pragma omp for reduction(+: soma)
            for(int i = 1; i <= numero; i++){

                if(numero % i == 0)
                    soma += i;

            }

    }
    fim = omp_get_wtime();

    tempoParalelo = fim - inicio;

    printf("Execução paralela usando o omp for:\n");
    printf("A soma dos divisores de %d é %ld.\n", numero, soma);
    printf("Tempo de execução: %lf.\n", tempoParalelo);
    printf("Speed up: %lf.\n", tempoSequencial / tempoParalelo);
    printf("Eficiência: %lf.\n\n", (tempoSequencial / tempoParalelo) / threads);

    soma = 0;

    inicio = omp_get_wtime();
    #pragma omp parallel num_threads(threads)
    {

        long int somar = 0;
        int ID = omp_get_thread_num();

        for(int i = ID; i < numero; i += threads){

            if(numero % (i + 1) == 0)
                somar += (i + 1);

        }

        #pragma omp critical
            soma += somar;

    }
    fim = omp_get_wtime();

    printf("Execução paralela usando o omp critical:\n");
    printf("A soma dos divisores de %d é %ld.\n", numero, soma);
    printf("Tempo de execução: %lf.\n", tempoParalelo);
    printf("Speed up: %lf.\n", tempoSequencial / tempoParalelo);
    printf("Eficiência: %lf.\n\n", (tempoSequencial / tempoParalelo) / threads);

    soma = 0;

    inicio = omp_get_wtime();
    #pragma omp parallel num_threads(threads)
    {

        long int somar = 0;
        int ID = omp_get_thread_num();

        for(int i = ID; i < numero; i += threads){

            if(numero % (i + 1) == 0)
                somar += (i + 1);

        }

        #pragma omp atomic
            soma += somar;

    }
    fim = omp_get_wtime();

    printf("Execução paralela usando o omp atomic:\n");
    printf("A soma dos divisores de %d é %ld.\n", numero, soma);
    printf("Tempo de execução: %lf.\n", tempoParalelo);
    printf("Speed up: %lf.\n", tempoSequencial / tempoParalelo);
    printf("Eficiência: %lf.\n\n", (tempoSequencial / tempoParalelo) / threads);

    return 0;
}