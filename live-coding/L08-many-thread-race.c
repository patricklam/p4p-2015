// credit: Josh
// observed output: 108

#include <stdlib.h>
#include <stdio.h>
#include <thread>
#include <mutex>
#include <unistd.h>
#include <iostream>

#define MAX_THREADS 8

static int counter = 0;

std::mutex m;

void loop()
{
    for (int i = 0; i < 100; i++)
    {
        int j = counter;
        j++;
        
        if (i % 10 == 0)
            sleep(1);
        counter = j;
    }
}

int main() {
    std::thread *tids[MAX_THREADS];
    for (int i = 0; i < MAX_THREADS; i++)
    {
        tids[i] = new std::thread(loop);
    }

    for (int j = 0; j < MAX_THREADS; j++)
    {
        tids[j]->join();
    }
    std::cout << counter;
}
