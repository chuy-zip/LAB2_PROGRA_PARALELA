#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

#define BUFFER_SIZE 4
#define NUM_ITEMS 20

int buffer[BUFFER_SIZE];
int count = 0;
int head = 0, tail = 0;

void producir(int item, int id) {
    // Esperar hasta que haya espacio en el buffer
    int inserted = 0;
    while (!inserted) {
        #pragma omp critical
        {
            if (count < BUFFER_SIZE) {
                buffer[tail] = item;
                tail = (tail + 1) % BUFFER_SIZE;
                count++;
                inserted = 1;
                printf("Productor %d produjo: %d (count: %d)\n", id, item, count);
            }
        }
    }
}

int consumir(int id) {
    int item = -1;
    int consumed = 0;
    
    // Esperar hasta que haya elementos en el buffer
    while (!consumed) {
        #pragma omp critical
        {
            if (count > 0) {
                item = buffer[head];
                head = (head + 1) % BUFFER_SIZE;
                count--;
                consumed = 1;
                printf("Consumidor %d consumió: %d (count: %d)\n", id, item, count);
            }
        }
        if (!consumed) {
            // Pequeña pausa para evitar busy waiting excesivo
            // busy waiting es cuando un proceso viene y se queda esperando a un recurso
            // en un ciclo pero sigue consumiendo recurso. Es una espera activa por así decirlo
            #pragma omp critical
            { /* Solo para mantener la estructura crítica */ }
            
        }
    }
    
    return item;
}

void procesar(int item, int id) {
    // esto solo es para simular un proceso
    double resultado = sqrt(item);
    printf("Consumidor %d procesó: %d -> %.2f\n", id, item, resultado);
}

int main() {
    printf("Iniciando producer-consumer con OpenMP\n");
    
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            // Hilo productor
            for (int i = 0; i < NUM_ITEMS; i++) {
                producir(i, omp_get_thread_num());
            }
        }
        
        #pragma omp section
        {
            // Hilo consumidor
            for (int i = 0; i < NUM_ITEMS; i++) {
                int item = consumir(omp_get_thread_num());
                procesar(item, omp_get_thread_num());
            }
        }
    }
    
    return 0;
}