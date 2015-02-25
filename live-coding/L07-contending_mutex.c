// ECE 459 live coding example:
// contended mutex; run time to observe its behaviour
//   [contrast with contending_spin.c]
// Typed by Patrick Lam.
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 8
#define BIG 104857600
static pthread_mutex_t mutex;

void *run(void *args) {
	int i;
        unsigned int seed;
        pthread_mutex_lock(&mutex);
	for(i = 0; i < BIG; i++)
	{
	    int r = rand_r(&seed);
	}
	pthread_mutex_unlock(&mutex);
}

int main()
{
	pthread_t t[NUM_THREADS];
        pthread_mutex_init(&mutex, 0);
	int i;

	for(i = 0; i < NUM_THREADS; i++)
		pthread_create(&t[i], NULL, &run, NULL);
	for(i = 0; i < NUM_THREADS; i++)
		pthread_join(t[i], NULL);

	pthread_mutex_destroy(&mutex);
	return 0;
		

}

