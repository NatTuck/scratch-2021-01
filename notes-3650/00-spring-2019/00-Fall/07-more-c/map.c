#include <stdio.h>

int 
add1(int xx)
{
    return xx + 1;
}

void
map(int (*op)(int), int* xs, int nn)
{
    for (int ii = 0; ii < nn; ++ii) {
        xs[ii] = op(xs[ii]);
    }
}

int
main(int _ac, char* _av[])
{
    int nums[] = {1, 2, 3, 4};

    map(add1, nums, 4);

    for (int ii = 0; ii < 4; ++ii) {
        printf("%d ", nums[ii]);
    }
    printf("\n");

    return 0;
}
