#include <stdio.h>

int gcd(int a, int b) {
    return a < b ? gcd(b, a) : b == 0 ? a : gcd(b, a % b);
}

int phi(int n) {
    int r = 0;
    for( int i=1; i<n; i++ )
        if ( gcd(n, i) == 1 ) r++;
    return r;
}

int main() {
    long long sum = 0;
    int low = 2;
    int high = 30000;

    for( int n=low; n<=high; n++ )
        sum += phi(n);

    printf("%lld\n", sum);

    return 0;
}

