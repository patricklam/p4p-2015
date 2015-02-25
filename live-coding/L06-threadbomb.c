// ECE 459 Lecture 6 live coding

// Determine max # of threads.
// Courtesy Sanjay Menakuru

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 100000L

void *fn(void *arg) {
    sleep(1);
    return arg;
}

int main() {
    pthread_t tids[NUM_THREADS] = {0};

    for (int i = 0; i < NUM_THREADS; ++i) {
        int res = pthread_create(&tids[i], 0, fn, 0);
        if (res) {
            printf("Got error on iteration %d: err=%d\n", i,  res);
            return 1;
        }
    }

    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(tids[i], NULL);
    }

    return 0;
}
