#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdatomic.h>

const int PP = 10; // number of threads
const long TOP = 1000000000; // a billoin

typedef struct sum_job {
    long r0;
    long r1;
    long* sum;
} sum_job;

void*
sum_range_thread(void* arg)
{
    sum_job* job = (sum_job*) arg;

    for (long ii = job->r0; ii < job->r1; ++ii) {
        if (ii % 101 == 0) {
            atomic_fetch_add(job->sum, ii);
        }
    }

    free(job);
    return 0;
}

pthread_t
spawn_sum_range(long r0, long r1, long* sum)
{
    sum_job* job = malloc(sizeof(sum_job));
    job->r0 = r0;
    job->r1 = r1;
    job->sum = sum;

    pthread_t tt;
    int rv = pthread_create(&tt, 0, sum_range_thread, job);
    assert(rv == 0);
    return tt;
}

void
join_sum_range(pthread_t thr)
{
    int rv = pthread_join(thr, 0);
    assert(rv == 0);
}

int
main(int _ac, char* _av[])
{
    pthread_t threads[PP];

    printf(
        "Summing numbers divisible"
        "by 101 from 0 to %ld.\n",
        TOP
    );

    long sum = 0;

    long step = TOP / PP;
    for (long ii = 0; ii < PP; ++ii) {
        long start = ii*step;
        long end = (ii+1)*step;
        threads[ii] = spawn_sum_range(start, end, &sum);
    }

    for (int ii = 0; ii < PP; ++ii) {
        join_sum_range(threads[ii]);
    }

    printf("sum = %ld\n", sum);
    return 0;
}
