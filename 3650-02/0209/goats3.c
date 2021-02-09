
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct goat {
    char* name;
    float mass; // kilos
    // 4 bytes of padding
} goat;

void
init_goat(goat* gg, const char* name, float mass)
{
    gg->name = strdup(name);
    gg->mass = mass;
}

goat
fattest_goat(goat* xs, long nn)
{
    goat fattest = xs[0];
    for (long ii = 1; ii < nn; ++ii) {
        if (fattest.mass < xs[ii].mass) {
            fattest = xs[ii];
        }
    }
    fattest.name = strdup(fattest.name);
    return fattest;
}

int
main(int argc, char* argv[])
{
    goat herd[4];
    // stack-allocated array of pointers
    // when you use them, arrays and pointers
    // are the same
    init_goat(&herd[0], "Alice", 20);
    init_goat(&herd[1], "Carol", 60);
    init_goat(&herd[2], "Dave", 55);
    init_goat(&herd[3], "Bob", 75);

    goat gg = fattest_goat(herd, 4);

    printf("The fattest goat is %s\n", gg.name);

    return 0;
}
