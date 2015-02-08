// ECE 459 Live Coding example
// Lecture 10
// Manually-parallelized vertical vector processing example;
// start 4 threads which each process a sub-array, 1 element at a time
//  $ gcc -O3 -o omp_vector_manual_vertical -std=c99 -pthread omp_vector_manual_vertical.c
//  $ /opt/oracle/solarisstudio12.3/bin/cc omp_vector_manual_vertical.c -o omp_vector_vertical -xO4 -lpthread

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS (4)
#define ARRAY_LENGTH (1024 * 1024)
#define ARRAY_OFFSET (ARRAY_LENGTH / NUM_THREADS)

double * vector;

void setup(int i) {
  for (int j = 0; j < 1000; j++)
    vector[i] += 1.0;
}

void* run(void* arg) {
  int id = (int)arg;
  for (int i = id*ARRAY_OFFSET; i < (id+1)*ARRAY_OFFSET; i++) {
    setup(i);
  }
}

int main() {
  vector = (double *) malloc(sizeof(double) * ARRAY_LENGTH);
  pthread_t threads[NUM_THREADS];

  for (int i = 0; i < NUM_THREADS; i++) {
    pthread_create(&threads[i], NULL, &run, (void*)i);
  }

  for (int i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }

  printf("vector[0] = %f\n", vector[0]);
}
