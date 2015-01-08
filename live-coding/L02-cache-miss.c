/* ECE 459 Lecture 2: importance of caches/prefetching on perf  */
/* requires -std=c99 for gcc to compile this */
/* thanks: Stephen Chou */

#include <stdio.h>
#include <stdlib.h>

#define SIZE (1 << 24)

int main() {
  int * a = calloc(SIZE, sizeof(int));
  int * b = calloc(1000, sizeof(int));
  srand(42);
  int c = 0, d = 0;
  
  for(int k = 0; k < 1000; ++k) {
    b[k] = rand() % SIZE;
  }
  for(int k = 0; k < SIZE; ++k) {
    a[k] = k;
  }
  for(int k = 0; k < 1000000; ++k) {
    c = 0;
    for(int n = 0; n < 1000; ++n) {
#ifdef FORCE_CACHE_MISS
      c += a[b[n]];
#else
      c += b[n];
#endif
    }
    d += c;
  }
  printf("%d\n", d);
  return 0;
}
