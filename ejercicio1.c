#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <stdlib.h>
#define N 500000

double arreglo[N];

void reset_array()
{
    for (int i = 0; i < N; i++)
    {
        arreglo[i] = i;
    }
}

void test_schedule(int num_hilos)
{
    omp_set_num_threads(num_hilos);

    // hay 3 tipos de schedule. Static, dynamic y guided
    // la idea de estafunción es hacer la prueba mas sencilla
    // para cada tipo de scheduling se hace la tarea con una cantidad de threads y con
    // esa cantidad de threads se prueban 2 chunkzises
    int chunk_size_1 = 16;
    int chunk_size_2 = 32;

    double tiempo_inicial, tiempo_final;

    
    printf("-------------------------------\n");
    printf("PRUEBAS CON: %d\n", num_hilos);


    // Ahora con static
    reset_array();
    tiempo_inicial = omp_get_wtime();
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < N; i++)
    {
        arreglo[i] = sqrt(arreglo[i]);
    }
    tiempo_final = omp_get_wtime();
    printf("Tiempo de ejecución con static paralelo, con %d hilos,: %.4f segundos\n", num_hilos, tiempo_final - tiempo_inicial);

    // static no usa chunk siez entonces toca el siguiente
    

    //---------------------------------------------------------------
    // Ahora con dynamic con chunk_size_1
    reset_array();
    tiempo_inicial = omp_get_wtime();
    #pragma omp parallel for schedule(dynamic, chunk_size_1)
    for (int i = 0; i < N; i++)
    {
        arreglo[i] = sqrt(arreglo[i]);
    }
    tiempo_final = omp_get_wtime();
    printf("Tiempo de ejecución con dynamic paralelo, con %d hilos y chunk_size %d: %.4f segundos\n", num_hilos, chunk_size_1, tiempo_final - tiempo_inicial);

    // Ahora con dynamic con chunk_size_2
    reset_array();
    tiempo_inicial = omp_get_wtime();
    #pragma omp parallel for schedule(dynamic, chunk_size_2)
    for (int i = 0; i < N; i++)
    {
        arreglo[i] = sqrt(arreglo[i]);
    }
    tiempo_final = omp_get_wtime();
    printf("Tiempo de ejecución con dynamic paralelo, con %d hilos y chunk_size %d: %.4f segundos\n", num_hilos, chunk_size_2, tiempo_final - tiempo_inicial);


    //---------------------------------------------------------------
    // Ahora con guided con chunk_size_1
    reset_array();
    tiempo_inicial = omp_get_wtime();
    #pragma omp parallel for schedule(guided, chunk_size_1)
    for (int i = 0; i < N; i++)
    {
        arreglo[i] = sqrt(arreglo[i]);
    }
    tiempo_final = omp_get_wtime();
    printf("Tiempo de ejecución con guided paralelo, con %d hilos y chunk_size %d: %.4f segundos\n", num_hilos, chunk_size_1, tiempo_final - tiempo_inicial);

    // Ahora con guided con chunk_size_2
    reset_array();
    tiempo_inicial = omp_get_wtime();
    #pragma omp parallel for schedule(guided, chunk_size_2)
    for (int i = 0; i < N; i++)
    {
        arreglo[i] = sqrt(arreglo[i]);
    }
    tiempo_final = omp_get_wtime();
    printf("Tiempo de ejecución con guided paralelo, con %d hilos y chunk_size %d: %.4f segundos\n", num_hilos, chunk_size_2, tiempo_final - tiempo_inicial);
}

int main()
{
    double tiempo_inicial, tiempo_final;

    printf("Programa pare 1, operaciones secuenciales\n");

    // parte 1
    // inicializar arreglo
    for (int i = 0; i < N; i++)
    {
        arreglo[i] = i;
    }

    tiempo_inicial = omp_get_wtime();
    // aplicar raiz en todos los elementos del arreglo
    for (int i = 0; i < N; i++)
    {
        arreglo[i] = sqrt(arreglo[i]);
    }

    tiempo_final = omp_get_wtime();
    printf("value = %.2lf \n", arreglo[100]);
    printf("Tiempo de ejecución secuencial: %.4f segundos\n", tiempo_final - tiempo_inicial);

    // parte 2 paralelizacion
    // re inicializar el arreglo
    reset_array();

    printf("\n\nPrograma parte 2, operaciones parelalas con Scheduling y Chunksize\n");
    int num_hilos = 4;
    int chunk_size = 16;

    printf("Hilos: %d\n", num_hilos);
    printf("Scheduler: dynamic, Chunk size: %d\n", chunk_size);

    // Ahora prueba paralela
    tiempo_inicial = omp_get_wtime();

    #pragma omp parallel for schedule(dynamic, chunk_size)
    for (int i = 0; i < N; i++)
    {
        arreglo[i] = sqrt(arreglo[i]);
    }

    tiempo_final = omp_get_wtime();
    printf("value = %.2lf \n", arreglo[100]);
    printf("Tiempo de ejecución paralelo: %.4f segundos\n", tiempo_final - tiempo_inicial);

    // parte 3, probar con distintas cantidades de hilos, scheduling y chunkzise
    printf("\n\nPrograma parte 3, operaciones parelalas con distintos Scheduling y Chunksize\n");
    int threads_to_test[4] = {2, 4, 6, 8};
    int array_len = sizeof(threads_to_test) / sizeof(threads_to_test[0]);

    for (int i = 0; i < array_len; i++){
        int thread_num = threads_to_test[i];
        test_schedule(thread_num);
    }
    

    return 0;
}