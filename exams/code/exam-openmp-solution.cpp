#include <vector>

int foo(int i);
int bar(int i);
int baz(int i);

void work(int * A, int * C, unsigned size)
{
  for (unsigned i = 1; i < size; ++i) {
    A[i] = foo(A[i-1]);
  }
  #pragma omp parallel for
  for (unsigned i = 0; i < size; ++i) {
    int b_even = bar(A[i]);
    int b_odd = baz(A[i]);
    if (b_even > b_odd) {
      C[i] = foo(A[i] * b_even);
    }
    else {
      C[i] = foo(A[i] - b_odd);
    }
  }
}

void swap(int * x, int * y);

void slow(unsigned iterations, int * A, int * C, unsigned size)
{
  for (unsigned i = 0; i < size; ++i) {
    A[i] = i;
  }
  for (unsigned i = 0; i < iterations; ++i) {
    if (i != 0) {
      swap(A, C);
    }
    work(A, C, size);
  }
}
