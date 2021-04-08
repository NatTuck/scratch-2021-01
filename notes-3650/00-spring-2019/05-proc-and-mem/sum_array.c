
#include <stdio.h>
#include <stdlib.h>

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
        puts("usage: ./sum_array N");
        return 1;
    }

    long nn = atol(argv[1]);
    long* xs = malloc(nn * sizeof(long));

    for (long ii = 0; ii < nn; ++ii) {
        int count = scanf("%ld", &(xs[ii]));
        if (count != 1) { abort(); }
    }

    printf("sum = %ld\n", sum_array(xs, nn));

    free(xs);
    return 0;
}



