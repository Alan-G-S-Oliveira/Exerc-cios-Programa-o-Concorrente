#include <stdio.h>
#include <locale.h>
#include <omp.h>

static long num_steps = 100000000;
double step;

int main(){

    setlocale(LC_ALL, "Portuguese");

    int i, num_threads, nprocs = omp_get_num_procs() / 2;
    double x, pi, sum = 0;
    double inicio, fim, tempoSequencial, tempoParalelo;

    step = 1 / (double) num_steps;

    inicio = omp_get_wtime();

    #pragma omp parallel private(x) num_threads(nprocs)
    {

        num_threads = omp_get_num_threads();

        #pragma omp for reduction(+: sum)

            for(i = 0; i < num_steps; i++){

                x = (i + 0.5) * step;
                sum = sum + 4 / (1 + x * x);

            }
    }

    fim = omp_get_wtime();

    tempoParalelo = fim - inicio;

    inicio = omp_get_wtime();

    sum = 0;
    for(i = 0; i < num_steps; i++){

        x = (i + 0.5) * step;
        sum = sum + 4 / (1 + x * x);

    }
    
    fim = omp_get_wtime();

    tempoSequencial = fim - inicio;

    pi = step * sum;

    printf("Aproximação de pi: %.10f\n", pi);
    printf("Tempo de execução sequencial: %f.\n", tempoSequencial);
    printf("Tempo de execução paralela com %d threads: %f.\n", num_threads, tempoParalelo);
    printf("Speed Up: %f.\n", tempoSequencial / tempoParalelo);
    printf("Eficiência: %f.\n\n", (tempoSequencial / tempoParalelo) / num_threads);

    return 0;
}