#include <pthread.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include <stdatomic.h>

// Number of threads
const int NN = 10;

// A billion.
const long TOP = 1000000000;

long sum;

typedef struct sum_job {
    long r0;
    long r1;
} sum_job;

void*
sum_range_thread(void* arg)
{
    sum_job job = *((sum_job*) arg);
    free(arg);

    for (long ii = job.r0; ii < job.r1; ++ii) {
        if (ii % 101 == 0) {
            atomic_fetch_add(&sum, ii);
        }
    }

    return 0;
}

pthread_t
spawn_sum_range(long r0, long r1)
{
    sum_job* job = malloc(sizeof(sum_job));
    job->r0 = r0;
    job->r1 = r1;

    pthread_t thread;
    int rv = pthread_create(&thread, 0, sum_range_thread, job);
    assert(rv == 0);
    return thread;
}

int
main(int _ac, char* _av[])
{
    pthread_t threads[NN];

    printf("Summing numbers divisible by 101 from 0 to %ld.\n", TOP - 1);

    sum = 0;

    long step = TOP / NN;
    for (int ii = 0; ii < NN; ++ii) {
        threads[ii] = spawn_sum_range(ii * step, (ii+1) * step);
    }

    for (int ii = 0; ii < NN; ++ii) {
        int rv = pthread_join(threads[ii], 0);
        assert(rv == 0);
    }

    printf("Sum = %ld\n", sum);

    return 0;
}


