#include <stdio.h>
#include <pthread.h>
#include <assert.h>

#define NN 10

void*
thread_main(void* ptr)
{
    int* xx = (int*) ptr;
    printf("We're in a thread: %d\n", *xx);
    *xx += 10;
    return xx;
}

int
main(int _argc, char* _argv[])
{
    int rv;

    // This array is going to be used by other
    // threads.
    // One index per thread.
    // No concurrent access to a given index.
    // This allocation outlives all other threads.
    int nums[NN];

    pthread_t threads[NN];

    printf("main: starting %d threads\n", NN);

    // ii
    // - Is shared memory
    // - We're sharing with other threads
    // - The other threads are writing
    // - Data racetastic
    for (int ii = 0; ii < NN; ++ii) {
        nums[ii] = ii;
        rv = pthread_create(
            &(threads[ii]), 0, thread_main, &(nums[ii])
        );
        assert(rv == 0);
    }

    printf("main: started %d threads\n", NN);

    for (int ii = 0; ii < NN; ++ii) {
        int* yy;
        rv = pthread_join(threads[ii], (void**) &yy);
        assert(rv == 0);
        printf("Thread %d return %d.\n", ii, *yy);
    }

    printf("main: finished %d threads\n", NN);

    return 0;
}
