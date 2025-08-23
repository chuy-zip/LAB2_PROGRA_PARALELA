#include <stdio.h>
#include <omp.h>

#define N 10000000

int arreglo[N];

// Nota gran aprte de este código es de mi implementacion dle lab anterior,
// donde también había sincronización con varuables compartidas

int main()
{
    omp_set_num_threads(8);

    for (int i = 0; i < N; i++)
    {
        arreglo[i] = i;
    }

    long long sum = 0;
    long long sum_paralel_red = 0;
    long long sum_paralel_atom = 0;
    long long sum_paralela_critic = 0;

    // ahora usando paralellismo con reduction
    double start_par_red = omp_get_wtime();

    #pragma omp parallel for reduction(+ : sum_paralel_red)
    for (int i = 0; i < N; i++)
    {
        sum_paralel_red += i;
    }

    double end_par_red = omp_get_wtime();

    printf("\nLa suma total usando paralelismo y reduction es: %lld \n", sum_paralel_red);
    printf("Tiempo real: %.4f segundos\n", end_par_red - start_par_red);

    // ahora usando paralellismo con atomic
    double start_par_atom = omp_get_wtime();

    #pragma omp parallel for
    for (int i = 0; i < N; i++)
    {
        #pragma omp atomic
        sum_paralel_atom += i;
    }

    double end_par_atom = omp_get_wtime();

    printf("\nLa suma total usando paralelismo y atomic es: %lld \n", sum_paralel_atom);
    printf("Tiempo real: %.4f segundos\n", end_par_atom - start_par_atom);

    // usando paralellismo con critical
    double start_par_crit = omp_get_wtime();

    #pragma omp parallel for
    for (int i = 0; i < N; i++)
    {
        #pragma omp critical
        {
            sum_paralela_critic += i;
        }
    }

    double end_par_crit = omp_get_wtime();

    printf("\nLa suma total usando paralelismo con critical es: %lld \n", sum_paralela_critic);
    printf("Tiempo real: %.4f segundos\n", end_par_crit - start_par_crit);

    return 0;
}