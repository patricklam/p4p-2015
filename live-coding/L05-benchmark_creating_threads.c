// ECE 459 Lecture 5 live coding
// demonstrates startup cost for threads vs forks

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUMFORKS 50000

void* do_nothing(void* name){
	int num = 0;
	pthread_exit(NULL);
}

void main(int args, char **stuff) {
	int i=0;
	pthread_t tid = 5;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	for(;i<NUMFORKS;i++) {
		int pid = pthread_create(&tid, &attr, do_nothing, NULL );
		if (pid) {
			printf("tid ERROR");
			exit(1);
		}
		int rc = pthread_join(tid, NULL);
		if (rc) {
			printf("rc ERROR");
			exit(1);
		}
	}
	pthread_attr_destroy(&attr);
	pthread_exit(NULL);
}
