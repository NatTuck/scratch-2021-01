#include <stdio.h>
#include <pthread.h>
#include <assert.h>

#define NN 10

void*
thread_main(void* thread_arg)
{
    int* xx = (int*)thread_arg;
    printf("Started thread %d\n", *xx);
    *xx += 10;
    return xx;
}

int
main(int _argc, char* _argv[])
{
    int rv;
    int nums[NN]; // Shared with threads
    pthread_t threads[NN];

    printf("main: (1) starting %d threads\n", NN);

    // What is this?
    //  void *(*start_routine) (void *)
    //  - pointer to a function
    //  - returns a void*
    //  - take a void*

    for (int ii = 0; ii < NN; ++ii) {
        nums[ii] = ii;
        rv = pthread_create(
            &(threads[ii]), 0, thread_main, &(nums[ii]));
        assert(rv == 0);
    }

    printf("main: (2) started %d threads\n", NN);

    for (int ii = 0; ii < NN; ++ii) {
        int* result;
        rv = pthread_join(threads[ii], (void**) &result);
        printf("thread %d return %d\n", ii, *result);
    }

    printf("main: (3) finished %d threads\n", NN);

    return 0;
}
