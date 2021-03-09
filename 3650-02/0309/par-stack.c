#include <pthread.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>

#define NN 1024

// .data
int stack[5];
int stptr = 0;    // where the next item goes
pthread_mutex_t mutex;
pthread_cond_t  condv;

void
stack_push(int xx)
{
    pthread_mutex_lock(&mutex);
    while (stptr >= 5) {
        pthread_cond_wait(&condv, &mutex);
    }
    stack[stptr++] = xx;
    pthread_cond_broadcast(&condv);
    pthread_mutex_unlock(&mutex);
}

int
stack_pop()
{
    pthread_mutex_lock(&mutex);
    while (stptr <= 0) {
        pthread_cond_wait(&condv, &mutex);
    }
    int yy = stack[--stptr];
    pthread_cond_broadcast(&condv);
    pthread_mutex_unlock(&mutex);
    return yy;
}

void*
producer_thread(void* arg)
{
    int nn = *((int*) arg);
    free(arg);

    for (int ii = 0; ii < nn; ++ii) {
        stack_push(ii);
    }
    return 0;
}

int
main(int _ac, char* _av[])
{
    pthread_mutex_init(&mutex, 0);
    pthread_cond_init(&condv, 0);

    pthread_t threads[2];

    for (int ii = 0; ii < 2; ++ii) {
        int* nn = malloc(sizeof(int));
        *nn = NN;
        int rv = pthread_create(
            &(threads[ii]),
            0,
            producer_thread,
            nn
        );
        assert(rv == 0);
    }

    for (int ii = 0; ii < (2*NN); ++ii) {
        int yy = stack_pop();
        printf("%d\n", yy);
    }

    for (int ii = 0; ii < 2; ++ii) {
        pthread_join(threads[ii], 0);
    }

    return 0;
}
