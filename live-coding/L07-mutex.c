#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 8

static int counter = 0;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *run(void *args) {
        int i, j = 0;
        for(i = 0; i < 100; i++)
        {
                pthread_mutex_lock(&mutex);
                j = counter;
                if (i % 10 == 0) sleep(1);
                j++;
                counter = j;
                pthread_mutex_unlock(&mutex);
        }
}

int main()
{
        pthread_t t[NUM_THREADS];
        int i;

        for(i = 0; i < NUM_THREADS; i++)
                pthread_create(&t[i], NULL, &run, NULL);
        for(i = 0; i < NUM_THREADS; i++)
                pthread_join(t[i], NULL);

        pthread_mutex_destroy(&mutex);
        printf("%d\n", counter);
        return 0;
                

}
