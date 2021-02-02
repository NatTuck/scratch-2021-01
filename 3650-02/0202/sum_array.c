
#include <stdio.h>
#include <stdlib.h>
#include <alloca.h>

long
sum_array(long* xs, long nn)
{
    long yy = 0;
    for (long ii = 0; ii < nn; ++ii) {
        yy += xs[ii];
    }
    return yy;
}

int
main(int argc, char* argv[])
{
    if (argc != 2) {
        puts("Usage: ./sum_array NN");
        return 1;
    }

    long nn = atol(argv[1]); // Number of items.

    //long xs[nn]; // a new C feature, introduced in 1999.
                   // Space for nn longs is allocated
                   // on the stack.
    long* xs = alloca(nn * sizeof(long));

    for (long ii = 0; ii < nn; ++ii) {
        int count = scanf("%ld", &(xs[ii]));
        if (count != 1) { abort(); }
    }

    printf("Sum = %ld\n", sum_array(xs, nn));

    return 0;
}

// C versions:
//  - Traditional C (didn't even have types for all function args)
//  - ANSI C, 1989 standard
//  - No new version for a decade until C99.
//  - Everyone was used to C being static, that C99 wasn't widely
//    supported until like 2012.
//  - More recent updates are better.
