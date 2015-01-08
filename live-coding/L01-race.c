/* ECE 459 Lecture 1 */
/* Demonstrates a race condition. */
/* compile with -pthread option */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int x;

void * subthread(void * x) {
  int * ip = x;
  *ip = 5;
  return NULL;
}

int main()
{
  x = 1;
  pthread_t tid;
  pthread_create(&tid, NULL, subthread, &x);
  sleep(1);
  printf("%d\n", x);
  return 0;
}
