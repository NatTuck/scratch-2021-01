#include <pthread.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

// Number of threads
const int NN = 10;

// A billion.
const long TOP = 1000000000;

typedef struct sum_job {
    long r0;
    long r1;
    long sum;
} sum_job;

void*
sum_range_thread(void* arg)
{
    sum_job* job = (sum_job*) arg;
    long sum = 0;
   
    for (long ii = job->r0; ii < job->r1; ++ii) {
        if (ii % 101 == 0) {
            sum += ii;
        }
    }

    job->sum = sum;
    return job;
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

long
join_sum_range(pthread_t thread)
{
    void* ret;
    int rv = pthread_join(thread, &ret);
    assert(rv == 0);

    sum_job* job = ((sum_job*) ret);
    long sum = job->sum;
    free(job);

    return sum;
}

int
main(int _ac, char* _av[])
{
    pthread_t threads[NN];

    printf("Summing numbers divisible by 101 from 0 to %ld.\n", TOP - 1);

    long step = TOP / NN;
    for (int ii = 0; ii < NN; ++ii) {
        long start = ii * step;
        threads[ii] = spawn_sum_range(ii, ii + step);
    }

    long sum = 0;
    for (int ii = 0; ii < NN; ++ii) {
        sum += join_sum_range(threads[ii]);
    }

    printf("Sum = %ld\n", sum);

    return 0;
}


