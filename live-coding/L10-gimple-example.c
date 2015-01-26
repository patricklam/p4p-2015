// to see gimple code, use: gcc gimple.c -O -o gimple -fdump-tree-all
// observe the optimizations

#include <stdio.h>
#include <stdlib.h>

int main(){
    int i, j, k;
    scanf("%d\n", &i);
    j = i;
    k = j * i + j;
    if (k + j == 2) {
        printf("%d\n", k);
    }
    j *= i;
}
