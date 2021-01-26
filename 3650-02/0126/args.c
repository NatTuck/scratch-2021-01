#include <stdio.h>

int
main(int argc, char* argv[])
{
    printf("%d args\n", argc);
    for (int ii = 0; ii < argc; ++ii) {
        printf(" - %d: [%s]\n", ii, argv[ii]);
    }
    return 0;
}
