
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

char**
reverse_string_array(char** xs, int nn)
{
    char** ys = malloc(nn * sizeof(char*));
    for (int ii = 0; ii < nn; ++ii) {
        ys[nn - ii - 1] = xs[ii];
    }
    return ys;
}

void
print_string_array(char** xs, int nn)
{
    for (int ii = 0; ii < nn; ++ii) {
        printf("%s\n", xs[ii]);
    }
}

char*
read_line()
{
    long  cap = 16;
    char* buf = malloc(cap);
    long  ii  = 0;

    for (;;) {
        if (ii >= cap) {
            cap *= 2;
            buf = realloc(buf, cap);
        }

        long count = read(0, buf + ii, 1);
        if (count == 0 || buf[ii] == '\n') {
            break;
        }
        ++ii;
    }
    buf[ii] = 0;

    char* yy = strdup(buf);
    free(buf);
    return yy;
}

int
main(int argc, char* argv[])
{
    if (argc != 2) {
        return 1;
    }

    long nn = atol(argv[1]);

    char* aa[nn];

    for (long ii = 0; ii < nn; ++ii) {
        aa[ii] = read_line();
    }

    char** bb = reverse_string_array(aa, nn);
    print_string_array(bb, nn);

    for (long ii = 0; ii < nn; ++ii) {
        free(aa[ii]);
    }

    free(bb);
    return 0;
}



