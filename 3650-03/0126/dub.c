#include <stdio.h>

long
dub(long xx)
{
    long yy = 2 * xx;
    printf("&yy = %p\n", &yy);
    return yy;
}

int
main(int argc, char* argv[])
{
    long aa = 10;
    printf("&aa = %p\n", &aa);
    long bb = dub(aa);
    printf("&bb = %p\n", &bb);
    printf("%ld\n", bb);
    return 0;
}
