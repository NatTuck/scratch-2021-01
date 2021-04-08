#include <stdio.h>

#include "vec.h"

vec*
read_vec()
{
    long xx;
    vec* ys = make_vec();
    while (1 == scanf("%ld", &xx)) {
        push(ys, xx);
    }
    return ys;
}

void
print_vec(vec* xs)
{
    for (long ii = 0; ii < xs->size; ++ii) {
        printf("%ld ", xs->data[ii]);
    }
    printf("\n");
}

int
main(int argc, char* argv[])
{
    vec* xs = read_vec();
    vec* ys = reverse(xs);
    print_vec(xs);
    print_vec(ys);
    free_vec(xs);
    free_vec(ys);
    return 0;
}


