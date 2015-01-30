#include <iostream>
#include <thread>
#include <mutex>
#define THREAD_COUNT 8

long long sum;
std::mutex sumLock;

int gcd(int a, int b) {
    return a < b ? gcd(b, a) : b == 0 ? a : gcd(b, a % b);
}

int phi(int n) {
    int r = 0;
    for( int i=1; i<n; i++ )
        if ( gcd(n, i) == 1 ) r++;
    return r;
}

void threadRoutine(int first, int last) {
    long long local_sum = 0;
    for( int n=first; n<=last; n++ ) {
        local_sum += phi(n);
    }

    sumLock.lock();
    sum += local_sum;
    sumLock.unlock();
}

int main() {
    std::thread threads[THREAD_COUNT];
    int low = 2;
    int high = 30000;
    int job_size = (high - low + 1) / THREAD_COUNT;
    int remaining = (high - low + 1) % THREAD_COUNT;

    sum = 0;

    int first = low;
    int last;

    for( int i=0; i<THREAD_COUNT; i++ ) {
        last = first + job_size - 1;

        if ( remaining > 0 ) {
            last++;
            remaining--;
        }

        threads[i] = std::thread(threadRoutine, first, last);

        first = last + 1; /* Move on to the next sub-interval */
    }

    for( int i=0; i<THREAD_COUNT; i++ )
        threads[i].join();

    std::cout << sum << std::endl;
    return 0;
}
