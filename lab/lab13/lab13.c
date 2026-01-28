#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "semaphore.h"

#define BUFFER_SIZE 9
#define PRODUCERS 2
#define CONSUMERS 3

int buffer[BUFFER_SIZE];
int readCount[BUFFER_SIZE];
int count = 0;

/* ВАЖНО: инициализация через конструктор */
Semaphore mutex(1);
Semaphore empty(BUFFER_SIZE);
Semaphore full(0);

/* PRODUCER */
void* producer(void* arg) {
    int id = *(int*)arg;
    int value = 0;

    while (1) {
        sleep(1);

        empty.p();
        mutex.p();

        buffer[count] = value;
        readCount[count] = 0;
        count++;

        printf("Producer %d produced %d (count=%d)\n", id, value, count);
        value++;

        mutex.v();
        full.v();
    }
}

/* CONSUMER */
void* consumer(void* arg) {
    int id = *(int*)arg;

    while (1) {
        sleep(1);

        full.p();
        mutex.p();

        int index = count - 1;

        if (readCount[index] < CONSUMERS) {
            readCount[index]++;
            printf("Consumer %d read %d (%d/3)\n",
                   id, buffer[index], readCount[index]);
        }

        if (readCount[index] == CONSUMERS && count > 3) {
            printf("Element %d removed\n", buffer[index]);
            count--;
            empty.v();
        } else {
            full.v();
        }

        mutex.v();
    }
}

int main() {
    pthread_t prod[PRODUCERS];
    pthread_t cons[CONSUMERS];

    int pid[PRODUCERS];
    int cid[CONSUMERS];

    for (int i = 0; i < PRODUCERS; i++) {
        pid[i] = i + 1;
        pthread_create(&prod[i], NULL, producer, &pid[i]);
    }

    for (int i = 0; i < CONSUMERS; i++) {
        cid[i] = i + 1;
        pthread_create(&cons[i], NULL, consumer, &cid[i]);
    }

    for (int i = 0; i < PRODUCERS; i++)
        pthread_join(prod[i], NULL);

    for (int i = 0; i < CONSUMERS; i++)
        pthread_join(cons[i], NULL);

    return 0;
}
