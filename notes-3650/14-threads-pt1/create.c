#include <stdio.h>
#include <pthread.h>
#include <assert.h>

#define NN 10

void*
thread_main(void* thread_arg)
{
    int xx = *((int*)thread_arg);
    printf("thread %d: We're in a thread.\n", xx);
    *((int*)thread_arg) += xx;
    return thread_arg;
}

int
main(int _argc, char* _argv[])
{
    int nums[NN];
    int rv;
    pthread_t threads[NN];

    printf("main: Starting %d threads.\n", NN);

    for (int ii = 0; ii < NN; ++ii) {
        nums[ii] = ii;

        rv = pthread_create(&(threads[ii]), 0, thread_main, &(nums[ii]));
        assert(rv == 0);
    }

    printf("main: Started %d threads.\n", NN);

    for (int ii = 0; ii < NN; ++ii) {
        void* ret;
        rv = pthread_join(threads[ii], &ret);

        int yy = *((int*) ret);
        printf("main: Joined thread %d, rv = %d.\n", ii, yy);
    }

    printf("main: All threads joined.\n");

    return 0;
}
