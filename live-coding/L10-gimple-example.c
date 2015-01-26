// to see gimple code, use: gcc gimple.c -O -o gimple -fdump-tree-all
// observe the optimizations

#include <stdio.h>
#include <stdlib.h>

int main(){
   int foo = 2;
   int bar = foo + 8;
   int res = bar%foo + bar;
   if(res == 0){
	res++;
   }
   printf("This is empty %n", bar);
}
