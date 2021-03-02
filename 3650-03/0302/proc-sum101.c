#include <stdio.h>
#include <assert.h>

#include <sys/mman.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int
main(int argc, char* argv[])
{
    // A billion
    const long TOP = 1000000000;
    const long NPP = TOP / 10;

    printf(
        "Summing numbers divisible by "
        "101 from 0 to %ld\n",
        TOP - 1
    );

    long* sum = mmap(
        0, sizeof(long), PROT_READ|PROT_WRITE,
        MAP_SHARED|MAP_ANONYMOUS, -1, 0
    );
    *sum = 0;

    pid_t kids[10];

    for (int pp = 0; pp < 10; ++pp) {
        if ((kids[pp] = fork())) {
            // do nothings
        }
        else {
            long i0 = pp*NPP;
            long iN = (pp+1)*NPP;

            for (long ii = i0; ii < iN; ++ii) {
                if (ii % 101 == 0) {
                    *sum += ii; // the problem
                    // This isn't atomic
    // - add %ii, (%sum)
    // That's misleading, the processor actually
    // splits this into three parts before
    // executing:
    //
    //    Process One               Process Two
    // - mov (%sum), %tmp
    //        10
    //                          - mov (%sum), %tmp
    //                                 10
    // - add %ii, %tmp
    //       101
    //                          - add %ii, %tmp
    //                                201
    // - mov %tmp, (%sum)
    //             111
    //                          - mov %tmp, (%sum)
    //                                      211
                }
            }

            munmap(sum, sizeof(long));
            exit(0);
        }
    }

    for (int pp = 0; pp < 10; ++pp) {
        waitpid(kids[pp], 0, 0);
    }

    printf("Sum = %ld\n", *sum);

    munmap(sum, sizeof(long));
    return 0;
}

// correct:    4950495450495045
// our answer: 2637101278159948
// answer2:    2439874240453964
