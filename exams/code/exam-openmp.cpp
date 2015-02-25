#include <vector>

int foo(int i);
int bar(int i);
int baz(int i);

void work(int * A, int * C, unsigned size)
{
  for (unsigned i = 1; i < size; ++i) {
    A[i] = foo(A[i-1]);
  }
  std::vector<int> B(size * 2); // vector of (size * 2) elements
  for (unsigned i = 0; i < size; ++i) {
    B[i*2] = bar(A[i]);
    B[i*2+1] = baz(A[i]); 
  }
  for (unsigned i = 0; i < size; ++i) {
    if (B[i*2] > B[i*2+1]) {
      C[i] = foo(A[i] * B[i*2]);
    }
    else {
      C[i] = foo(A[i] - B[i*2+1]);
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
