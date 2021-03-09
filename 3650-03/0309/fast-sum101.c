#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <stdlib.h>

// Ten threads
const long NN = 10;
// A billion
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
    int rv = pthread_create(
        &tt, 0, sum_range_thread, job);
    assert(rv == 0);
    return tt;
}

long
join_sum_range(pthread_t tt)
{
    sum_job* job;
    int rv = pthread_join(tt, (void**) &job);
    assert(rv == 0);
    long sum = job->sum;
    free(job);
    return sum;
}

int
main(int _ac, char* _av[])
{
    pthread_t threads[NN];

    long step = TOP / NN;

    for (int ii = 0; ii < NN; ++ii) {
        long r0 = step * ii;
        long r1 = step * (ii+1);
        threads[ii] = spawn_sum_range(r0, r1);
    }


    long sum = 0;
    for (int ii = 0; ii < NN; ++ii) {
        sum += join_sum_range(threads[ii]);
    }

    printf("sum = %ld\n", sum);
    return 0;
}
