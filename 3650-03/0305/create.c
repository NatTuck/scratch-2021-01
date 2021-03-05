#include <stdio.h>
#include <pthread.h>
#include <assert.h>

    // void *(*start_routine) (void *)

    // pointer to a function
    //  - args are in parens (void*)
    //    - one arg, void*
    //  - return type first
    //    - void*

#define NN 10

void*
thread_main(void* ptr)
{
    int* xx = (int*)ptr;
    printf("In thread: %d\n", *xx);
    *xx += 10;
    return xx;
}

int
main(int _ac, char* _av[])
{
    int rv;

    int nums[NN];
    pthread_t threads[NN];

    printf("main: (1) Starting %d threads.\n", NN);


    for (int ii = 0; ii < NN; ++ii) {
        nums[ii] = ii;
        rv = pthread_create(
            &(threads[ii]), 0, thread_main, &(nums[ii])
        );
        assert(rv == 0);
    }

    printf("main: (2) Started %d threads.\n", NN);

    for (int ii = 0; ii < NN; ++ii) {
        int* tmp;
        rv = pthread_join(threads[ii], (void**) &tmp);
        assert(rv == 0);
        printf("Thread %d returned %d\n", ii, *tmp);
    }

    printf("main: (3) Finished %d threads.\n", NN);

    return 0;
}
