
#include <stdlib.h>
#include <stdio.h>

long
iterate(long x)
{
    if (x % 2 == 0) {
        return x / 2;
    }
    else {
        return x*3 + 1;
    }
}

int
main(int argc, char* argv[])
{
    long xx = atol(argv[1]);
    long ii = 0;

    while (xx > 1) {
        printf("%ld\n", xx);
        xx = iterate(xx);
        ii++;
    }

    printf("%ld\n", ii);
    return 0;
}
