#include <stdio.h>
#include <stdlib.h>

const long NN = 500 * 1000 * 1000;

int
main(int argc, char* argv[])
{
    int* xs = malloc(NN * sizeof(int));
    int* ys = malloc(NN * sizeof(int));
    int* zs = malloc(NN * sizeof(int));

    // How big is each array?
    //   - 2GB per array

    for (long ii = 0; ii < NN; ++ii) {
        xs[ii] = ii;
        ys[ii] = ii;
        zs[ii] = 0;
    }

    for (long ii = 0; ii < NN; ++ii) {
        zs[ii] = xs[ii] + ys[ii];
    }

    printf("%d\n", zs[1024]);
    printf("%d\n", zs[NN - 4]);

    free(zs);
    free(ys);
    free(xs);
}
