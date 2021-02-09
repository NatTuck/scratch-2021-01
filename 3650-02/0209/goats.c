
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct goat {
    char* name;
    float mass; // kilos
    // 4 bytes of padding
} goat;

goat*
make_goat(const char* name, float mass)
{
    goat* gg = malloc(sizeof(goat));
    gg->name = strdup(name);
    gg->mass = mass;
    return gg;
}

void
free_goat(goat* gg)
{
    free(gg->name);
    free(gg);
}

goat*
fattest_goat(goat** xs, long nn)
{
    goat* fattest = xs[0];
    for (long ii = 1; ii < nn; ++ii) {
        if (fattest->mass < xs[ii]->mass) {
            fattest = xs[ii];
        }
    }
    return fattest;
}

int
main(int argc, char* argv[])
{
    goat* herd[4];
    // stack-allocated array of pointers
    // when you use them, arrays and pointers
    // are the same
    herd[0] = make_goat("Alice", 20);
    herd[1] = make_goat("Bob", 75);
    herd[2] = make_goat("Carol", 60);
    herd[3] = make_goat("Dave", 55);

    goat* gg = fattest_goat(herd, 4);

    for (long ii = 0; ii < 4; ++ii) {
        free_goat(herd[ii]);
    }

    printf("The fattest goat is %s\n", gg->name);

    return 0;
}
