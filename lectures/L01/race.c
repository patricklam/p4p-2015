#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>


void * subthread(void* x) {
    int * ip = x;
    *ip = 5;
}

int i;
int main()
{
    pthread_t tid;
    pthread_create(&tid, NULL, &subthread, &i);
    i = 1;
    sleep(1);
    printf("%d\n", i);
}
