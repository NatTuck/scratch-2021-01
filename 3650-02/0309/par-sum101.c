#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <stdlib.h>

const int PP = 10; // number of threads
const long TOP = 1000000000; // a billoin

typedef struct sum_job {
    long r0;
    long r1;
    long sum;
} sum_job;

void*
sum_range_thread(void* arg)
{
    sum_job* job = (sum_job*) arg;

    job->sum = 0;

    for (long ii = job->r0; ii < job->r1; ++ii) {
        if (ii % 101 == 0) {
            job->sum += ii;
        }
    }

    return job;
}

pthread_t
spawn_sum_range(long r0, long r1)
{
    sum_job* job = malloc(sizeof(sum_job));
    job->r0 = r0;
    job->r1 = r1;

    pthread_t tt;
    int rv = pthread_create(&tt, 0, sum_range_thread, job);
    assert(rv == 0);
    return tt;
}

long
join_sum_range(pthread_t thr)
{
    sum_job* job;
    int rv = pthread_join(thr, (void**) &job);
    assert(rv == 0);
    long sum = job->sum;
    free(job);
    return sum;
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


    long step = TOP / PP;
    for (long ii = 0; ii < PP; ++ii) {
        long start = ii*step;
        long end = (ii+1)*step;
        threads[ii] = spawn_sum_range(start, end);
    }

    long sum = 0;
    for (int ii = 0; ii < PP; ++ii) {
        sum += join_sum_range(threads[ii]);
    }

    printf("sum = %ld\n", sum);
    return 0;
}
