// compile this file with:
//     gcc -o isort isort.c
//
// run with
//    ./isort

#include <stdio.h>
#include <stdlib.h>

int*
read_ints(int nn)
{
    int* ys = malloc(nn * sizeof(int)); // sbrk
    int y;

    for (int ii = 0; ii < nn; ++ii) {
        scanf("%d", &y); // read int
        ys[ii] = y;
    }

    return ys;
}

void
print_ints(int* xs, int nn)
{
    for (int ii = 0; ii < nn; ++ii) {
        printf("%d ", xs[ii]); // print int; print string
    }
    printf("\n"); // print string
}

// insert a number into an array where
// the first nn elements are already sorted
void
insert(int* xs, int nn, int xx) {
    int yy;
    int ii;

    for (ii = 0; ii < nn; ++ii) {
        yy = xs[ii];
        if (xx < yy) {
            xs[ii] = xx;
            xx = yy;
        }
    }

    xs[ii] = xx;
}

int*
isort(int* xs, int nn) {
    //int* ys = malloc(nn * sizeof(int));
    int* ys = (int*)20;

    for (int ii = 0; ii < nn; ++ii) {
        insert(ys, ii, xs[ii]);
    }

    return ys;
}


int
main(int _ac, char* _av[])
{
    int nn;

    printf("How many ints?\n"); // print string
    scanf("%d", &nn); // read int

    printf("Enter that many ints, one per line\n"); // print string
    int* xs = read_ints(nn);

    int* ys = isort(xs, nn); // print string

    printf("Your array:\n"); // print string
    print_ints(xs, nn);

    printf("Sorted array:\n"); // print string
    print_ints(ys, nn);

    return 0;
}
