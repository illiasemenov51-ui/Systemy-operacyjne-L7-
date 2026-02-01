#include <iostream>
#include <vector>
#include <cstdlib>
#include <pthread.h>
#include <unistd.h>
#include "semaphore.h"

using namespace std;

const int BUFFER_SIZE = 9;
const int THREADS_COUNT = 4;

/* ================= BUFFER ================= */

class Buffer
{
private:
    vector<int> values;

    Semaphore mutex;   // sekcja krytyczna
    Semaphore empty;   // wolne miejsca
    Semaphore semA;    // konsument A
    Semaphore semB;    // konsument B

    void print(const string& who)
    {
        cout << who << " | size=" << values.size() << " [ ";
        for (int v : values) cout << v << " ";
        cout << "]" << endl;
    }

public:
    Buffer() :
        mutex(1),
        empty(BUFFER_SIZE),
        semA(0),
        semB(0)
    {}

    bool canA()
    {
        return values.size() > 4 &&
               values.front() % 3 == 0;
    }

    bool canB()
    {
        return values.size() > 4 &&
               values.front() % 3 != 0;
    }

    void put(int value)
    {
        empty.p();
        mutex.p();

        values.push_back(value);
        print("PROD");

        if (canA()) semA.v();
        if (canB()) semB.v();

        mutex.v();
    }

    int getA()
    {
        semA.p();
        mutex.p();

        int v = values.front();
        values.erase(values.begin());
        empty.v();

        print("A");

        if (canA()) semA.v();
        if (canB()) semB.v();

        mutex.v();
        return v;
    }

    int getB()
    {
        semB.p();
        mutex.p();

        int v = values.front();
        values.erase(values.begin());
        empty.v();

        print("B");

        if (canA()) semA.v();
        if (canB()) semB.v();

        mutex.v();
        return v;
    }
};

/* =============== GLOBAL BUFFER =============== */

Buffer buffer;

/* ================= THREADS ================= */

void* producer(void* arg)
{
    for (int i = 0; i < 15; i++)
    {
        int value = rand() % 90 + 10; // ładniejsze liczby
        buffer.put(value);
        sleep(1);
    }
    return NULL;
}

void* consumerA(void* arg)
{
    for (int i = 0; i < 15; i++)
    {
        buffer.getA();
        sleep(1);
    }
    return NULL;
}

void* consumerB(void* arg)
{
    for (int i = 0; i < 15; i++)
    {
        buffer.getB();
        sleep(1);
    }
    return NULL;
}

/* ================= MAIN ================= */

int main()
{
    srand(time(NULL));

    pthread_t tid[THREADS_COUNT];

    pthread_create(&tid[0], NULL, producer, NULL);
    pthread_create(&tid[1], NULL, producer, NULL);
    pthread_create(&tid[2], NULL, consumerA, NULL);
    pthread_create(&tid[3], NULL, consumerB, NULL);

    for (int i = 0; i < THREADS_COUNT; i++)
        pthread_join(tid[i], NULL);

    return 0;
}
