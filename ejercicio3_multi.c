#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define BUFFER_SIZE 3
#define NUM_PRODUCERS 2
#define NUM_CONSUMERS 2
#define ITEMS_PER_PRODUCER 15

int buffer[BUFFER_SIZE];
int count = 0;
int head = 0, tail = 0;

void producir(int item, int id) {
    int inserted = 0;
    while (!inserted) {
        #pragma omp critical
        {
            if (count < BUFFER_SIZE) {
                buffer[tail] = item;
                tail = (tail + 1) % BUFFER_SIZE;
                count++;
                inserted = 1;
                printf("P%d produjo: %d (buffer: %d/%d)\n", id, item, count, BUFFER_SIZE);
            }
        }
    }
}

int consumir(int id) {
    int item = -1;
    int consumed = 0;
    
    while (!consumed) {
        #pragma omp critical
        {
            if (count > 0) {
                item = buffer[head];
                head = (head + 1) % BUFFER_SIZE;
                count--;
                consumed = 1;
                printf("C%d consumió: %d (buffer: %d/%d)\n", id, item, count, BUFFER_SIZE);
            }
        }
    }
    
    return item;
}

int main() {
    printf("Iniciando con %d productores y %d consumidores\n", NUM_PRODUCERS, NUM_CONSUMERS);
    
    #pragma omp parallel num_threads(NUM_PRODUCERS + NUM_CONSUMERS)
    {
        int id = omp_get_thread_num();
        
        if (id < NUM_PRODUCERS) {
            // Hilos productores
            for (int i = 0; i < ITEMS_PER_PRODUCER; i++) {
                int item = id * ITEMS_PER_PRODUCER + i;
                producir(item, id);
            }
        } else {
            // Hilos consumidores
            int consumidos = 0;
            while (consumidos < (NUM_PRODUCERS * ITEMS_PER_PRODUCER) / NUM_CONSUMERS) {
                int item = consumir(id - NUM_PRODUCERS);
                // Procesar el item
                consumidos++;
            }
        }
    }
    
    return 0;
}