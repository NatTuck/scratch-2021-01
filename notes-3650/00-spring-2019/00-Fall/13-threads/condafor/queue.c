#include <pthread.h>
#include <semaphore.h>
#include <stdatomic.h>
#include <assert.h>
#include <unistd.h>
#include <stdio.h>

#define QUEUE_SIZE 8

int queue[QUEUE_SIZE];
unsigned int qii = 0; // Input index
unsigned int qjj = 0; // Output index
sem_t isem; // Space
sem_t osem; // Items

int
queue_get()
{
    int rv;
    rv = sem_wait(&osem);
    assert(rv == 0);

    unsigned int ii = atomic_fetch_add(&qii, 1);
    int yy = queue[ii % QUEUE_SIZE];

    rv = sem_post(&isem);
    assert(rv == 0);

    return yy;
}

void
queue_put(int xx)
{
    int rv;
    rv = sem_wait(&isem);
    assert(rv == 0);

    unsigned int jj = atomic_fetch_add(&qjj, 1);
    queue[jj % QUEUE_SIZE] = xx;

    rv = sem_post(&osem);
    assert(rv == 0);
}

void*
producer_thread(void* _arg)
{
    int ii = 0;
    while (1) {
        queue_put(ii++);
    }
    return 0;
}

int
main(int _ac, char* _av[])
{
    pthread_t threads[2];

    sem_init(&isem, 0, QUEUE_SIZE);
    sem_init(&osem, 0, 0);

    for (int ii = 0; ii < 2; ++ii) {
        int rv = pthread_create(&(threads[ii]), 0, producer_thread, 0);
        assert(rv == 0);
    }

    while (1) {
        int yy = queue_get();
        printf("%d\n", yy);
        usleep(10000);
    }

    return 0;
}
