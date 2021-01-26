
#include <stdio.h>

long
dub(long xx)
{
    // xx is passed in register
    // can't take address
    // need to copy to stack
    // we'll make that local variable

    printf("&xx = %p\n", &xx);
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
    printf("bb = %ld\n", bb);
    return 0;
}
