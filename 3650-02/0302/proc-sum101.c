#include <stdio.h>
#include <assert.h>

#include <sys/mman.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>


int
main(int _ac, char* _av[])
{
    // A Billion
    const long TOP = 1000000000;
    const long NPP = TOP / 10;

    printf(
        "Summing numbers divisible by 101 from 0 to %ld\n",
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
            // do nothing
        }
        else {
            long i0 = NPP*pp;
            long iN = NPP*(pp+1);

            for (long ii = i0; ii < iN; ++ii) {
                if (ii % 101 == 0) {
                    *sum += ii; // This is a problem.
  // += here is not a single instruction
  // instead, it's something like
  //    - add %ii, (%sum)  // intel is lying to us
  //   this is decompsed to something like this:
  //
  //    process one:             process two:
  //
  //    - mov (%sum), %t0
  //           10
  //                             - mov (%sum), %t0
  //                                   10
  //    - add %ii, %t0
  //           101
  //                             - add %ii, %t0
  //                                   201
  //    - mov %t0, (%sum)
  //               111
  //                             - mov %t0, (%sum)
  //                                        211
                }
            }

            munmap(sum, sizeof(long));
            exit(0);
        }
    }

    for (long pp = 0; pp < 10; ++pp) {
        waitpid(kids[pp], 0, 0);
    }

    printf("Sum = %ld\n", *sum);

    munmap(sum, sizeof(long));
    return 0;
}

// Seq result: 4950495450495045 (consistent)
// Proc result: 2521519641260378 (varying slightly)
