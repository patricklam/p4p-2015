/* ECE 459 Lecture 2: branch mispredicts */
/* gcc -O2 L02-branch-mispredict.c -DEXPECT_RESULT={0,1} -o L02-branch-mispredict */
/* gcc 4.7 and 4.8 show perf differences; gcc-4.9 ignores the bogus hint. */
/* credit: blog.man7.org/2012/10/how-much-do-builtinexpect-likely-and.html */
#include <stdlib.h>
#include <stdio.h>

static __attribute__ ((noinline)) int f(int a) { return a; }

#define BSIZE 1000000

int main(int argc, char* argv[])
{
  int *p = calloc(BSIZE, sizeof(int));
  int j, k, m1 = 0, m2 = 0;
  for (j = 0; j < 1000; j++) {
    for (k = 0; k < BSIZE; k++) {
      if (__builtin_expect(p[k], EXPECT_RESULT)) {
        m1 = f(++m1);
      } else {
        m2 = f(++m2);
      }
    }
  }

  printf("%d, %d\n", m1, m2);
}
