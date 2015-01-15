// ECE 459 Lecture 5 live coding

// Determine max # of threads.
// Courtesy David Hu.

#include <stdio.h>
#include <stdlib.h>

#define MAX_ITERATIONS 100000

int dummy_thread_func() {
    sleep(1);
}

int main() {
	int i = 0;
	int j;
	pthread_t thread_ids[MAX_ITERATIONS];

	for (; i < MAX_ITERATIONS; i++) {
		int status = pthread_create(&thread_ids[i], NULL, &dummy_thread_func, NULL);
		
		if (status) {
			printf("Stopped at iteration %d\n", i);
			exit(1);
		}
	}

	for (j = 0; j < MAX_ITERATIONS; j++) {
		pthread_join(thread_ids[j], NULL);
	}
 
	return 0;
}
