
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// What's sizeof(goat) ?
//  - 16 bits = 2 bytes <-
//  - 12 bytes = pointer is 8, float is 4
//  - 16 bytes
typedef struct goat {
    char* name;
    float mass; // kg
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
    goat fattest = *xs[0];
    for (long ii = 1; ii < nn; ++ii) {
        if (xs[ii]->mass > fattest.mass) {
            fattest = *xs[ii];
        }
    }
    return make_goat(fattest.name, fattest.mass);
}

int
main(int _ac, char* _av[])
{
    goat* herd[4];
    herd[0] = make_goat("Alice", 50);
    herd[1] = make_goat("Bob", 75);
    herd[2] = make_goat("Carol", 60);
    herd[3] = make_goat("Dave", 55);

    goat* gg = fattest_goat(herd, 4);

    for (int ii = 0; ii < 4; ++ii) {
        free_goat(herd[ii]);
    }

    printf("Fattest goat is %s\n", gg->name);

    printf("sizeof(goat) = %ld\n", sizeof(goat));

    free_goat(gg);

    return 0;
}
