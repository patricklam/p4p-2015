// ECE 459 Live Coding example
// Lecture 10
// Manually-parallelized good horizontal vector processing example;
// start 4 threads which each process (1000 times) a sub-array.
//  $ gcc -O3 -o omp_vector_manual_goodstuff -std=c99 -pthread omp_vector_manual_goodstuff.c
//  $ /opt/oracle/solarisstudio12.3/bin/cc omp_vector_manual_goodstuff.c -o omp_vector_goodstuff -xO4 -lpthread

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
  for (int i = 0; i < 1000; i++) {
    setup(id, 1024*1024);
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
