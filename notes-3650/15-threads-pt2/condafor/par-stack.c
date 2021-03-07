#include <pthread.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>

#define STACK_SIZE 5
int stack[STACK_SIZE];
int stptr = 0;
pthread_mutex_t mutex;
pthread_cond_t  condv;

void
stack_push(int xx)
{
    pthread_mutex_lock(&mutex);
    while (stptr >= STACK_SIZE) {
        pthread_cond_wait(&condv, &mutex);
    }
    stack[++stptr] = xx;
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
    int yy = stack[stptr--];
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
}

int
main(int _ac, char* _av[])
{
    pthread_t threads[2];
    pthread_mutex_init(&mutex, 0);
    pthread_cond_init(&condv, 0);

    for (int ii = 0; ii < 2; ++ii) {
        int* nn = malloc(sizeof(int));
        *nn = 1000;
        int rv = pthread_create(&(threads[ii]), 0, producer_thread, nn);
        assert(rv == 0);
    }

    while (1) {
        int yy = stack_pop();
        printf("%d\n", yy);
        usleep(10000);
    }

    return 0;
}
