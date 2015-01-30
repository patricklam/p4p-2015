#include <stdio.h>
#include <pthread.h>
#define THREAD_COUNT 8

typedef struct { int first, last; } pair;

long long sum;
pthread_mutex_t sumLock;

int gcd(int a, int b) {
    return a < b ? gcd(b, a) : b == 0 ? a : gcd(b, a % b);
}

int phi(int n) {
    int i, r = 0;
    for( i=1; i<n; i++ )
        if ( gcd(n, i) == 1 ) r++;
    return r;
}

void* threadRoutine(void* arg)
{
    pair p = *((pair*)arg);
    int n;

    long long local_sum = 0;
    for( n=p.first; n<=p.last; n++ ) {
        local_sum += phi(n);
    }

    pthread_mutex_lock(&sumLock);
    sum += local_sum;
    pthread_mutex_unlock(&sumLock);
}

int main()
{
    pthread_t tid[THREAD_COUNT];                     /**< ID of worker threads */
    pair intervals[THREAD_COUNT];                    /**< Sub-intervals that are passed to
                                                          threads */
    int low = 2;                                     /**< Input Interval lower value */
    int high = 30000;                                /**< Input Interval higher value */
    int job_size = (high - low + 1) / THREAD_COUNT;  /**< Number of numbers in the sub-interval
                                                          for each thread */
    int remaining = (high - low + 1) % THREAD_COUNT; /**< Number of remaining jobs which should
                                                          be distributed over other sub-intervals */

    int i, first = low, last;

    for( i=0; i<THREAD_COUNT; i++ ) {
        intervals[i].first = first;
        intervals[i].last = first + job_size - 1;

        if ( remaining > 0 ) {
            intervals[i].last++;
            remaining--;
        }

        first = intervals[i].last + 1;
    }

    sum = 0;
    pthread_mutex_init(&sumLock, NULL);

    for( i=0; i<THREAD_COUNT; i++ ) {
        pthread_create(&tid[i], NULL, threadRoutine, &intervals[i]);
    }

    for( i=0; i<THREAD_COUNT; i++ )
        pthread_join(tid[i], NULL);

    printf("%lld\n", sum);

    return 0;
}

