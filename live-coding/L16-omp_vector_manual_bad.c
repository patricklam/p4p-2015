// ECE 459 Live Coding example
// Lecture 10
// Manually-parallelized bad horizontal vector processing example;
// 1000 times, start 4 threads which each process a sub-array.
//  $ gcc -O3 -o omp_vector_manual_bad -std=c99 -pthread omp_vector_manual_bad.c
//  $ /opt/oracle/solarisstudio12.3/bin/cc omp_vector_manual_bad.c -o omp_vector_bad -xO4 -lpthread

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS (4)
#define ARRAY_LENGTH (1024 * 1024)
#define ARRAY_OFFSET (ARRAY_LENGTH / NUM_THREADS)

double * vector;

void setup(int id, int length) {
  int i;
  for (i = id * ARRAY_OFFSET; i < (id + 1) * ARRAY_OFFSET; i++) {
    vector[i] += 1.0;
  }
}

void* run(void* arg) {
  int id = (int)arg;
  setup(id, 1024*1024);
}

int main() {
  vector = (double *) malloc(sizeof(double) * ARRAY_LENGTH);
  pthread_t threads[NUM_THREADS];

  for (int q = 0; q < 1000; q++) {
      for (int i = 0; i < NUM_THREADS; i++) {
          pthread_create(&threads[i], NULL, &run, (void*)i);
      }
  }

  for (int i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }

  printf("vector[0] = %f\n", vector[0]);
}
