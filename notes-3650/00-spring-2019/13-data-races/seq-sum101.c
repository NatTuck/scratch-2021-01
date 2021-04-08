#include <stdio.h>
#include <assert.h>

// A billion.
const long TOP = 1000000000;

int
main(int _ac, char* _av[])
{
    printf("Summing numbers divisible by 101 from 0 to %ld.\n", TOP - 1);

    long sum = 0;
    for (long ii = 0; ii < TOP; ++ii) {
        if (ii % 101 == 0) {
            sum += ii;
        }
    }

    printf("Sum = %ld\n", sum);
    return 0;
}


