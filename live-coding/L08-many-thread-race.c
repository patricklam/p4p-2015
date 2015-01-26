// credit: Josh
// observed output: 108

// I've modified this to use different locking mechanisms.
// Run & time it with different compile flags to test that out.
// To observe time differences, `i' should go to something like 10000000, not 100.
// Raw compilation is racy and is fastest.
// Atomics are slightly slower.
// Mutexes are slowest.

#include <stdlib.h>
#include <stdio.h>
#include <thread>
#ifdef MUTEX
#include <mutex>
#endif
#ifdef ATOMIC
#include <atomic>
#endif
#include <unistd.h>
#include <iostream>

#define MAX_THREADS 8

#ifdef ATOMIC
static std::atomic<int> counter;
#else
static int counter = 0;
#endif

#ifdef MUTEX
std::mutex m;
#endif

void loop()
{
    for (int i = 0; i < 100; i++)
    {
#ifdef MUTEX
        m.lock();
#endif
#ifndef ATOMIC
        int j = counter;
        j++;        
        counter = j;
#else
        counter++;
#endif
#ifdef MUTEX
        m.unlock();
#endif
    }
}

int main() {
    std::thread *tids[MAX_THREADS];
    counter = 0;
    for (int i = 0; i < MAX_THREADS; i++)
    {
        tids[i] = new std::thread(loop);
    }

    for (int j = 0; j < MAX_THREADS; j++)
    {
        tids[j]->join();
    }
    std::cout << counter << "\n";
}
