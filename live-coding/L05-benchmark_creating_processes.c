// ECE 459 Lecture 5 live coding
// demonstrates startup cost for threads vs forks

#include <stdio.h>
#include <stdlib.h>

#define NUMFORKS 50000

void do_nothing()
{
	int num = 0;
}

void main(int args, char **stuff) {
	int i=0;
	for(;i<NUMFORKS;i++) {
		int pid = fork();
		if (pid == 0) {
			do_nothing();
			exit(0);
		}
		else if (pid>0) {
			int status = -1;
			waitpid(pid, status, 0);
		}
		else {
			printf("ERROR");
			exit(1);
		}
	}
}
