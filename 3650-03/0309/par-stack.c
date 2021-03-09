#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <pthread.h>

// .data
int stack[5];
int stptr = 0;  // where the next item goes
pthread_mutex_t lock;
pthread_cond_t condv;
// There's proably other vars
// from libraries in .data

void
stack_push(int xx)
{
    pthread_mutex_lock(&lock);
    while (stptr >= 4) {
        pthread_cond_wait(&condv, &lock);
    }
    stack[stptr++] = xx;
    pthread_cond_broadcast(&condv);
    pthread_mutex_unlock(&lock);
}

int
stack_pop()
{
    pthread_mutex_lock(&lock);
    while (stptr <= 0) {
        pthread_cond_wait(&condv, &lock);
    }
    int yy = stack[--stptr];
    pthread_cond_broadcast(&condv);
    pthread_mutex_unlock(&lock);
    return yy;
}

void*
producer_thread(void* arg)
{
    long nn = (long) arg;
    for (int ii = 0; ii < nn; ++ii) {
        stack_push(ii);
    }
    return 0;
}

#define NN 1024

int
main(int _ac, char* _av[])
{
    pthread_mutex_init(&lock, 0);
    pthread_t threads[2];

    for (int ii = 0; ii < 2; ++ii) {
        int rv = pthread_create(
            &(threads[ii]), 0,
            producer_thread, (void*) NN);
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
