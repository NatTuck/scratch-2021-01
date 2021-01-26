#include <stdio.h>

int
 //     %rdi        %rsi        %rdx, %rcx, %r8, %r9
main(int argc, char* argv[])
{
    printf("argc = %d\n", argc);
    for (long ii = 0; ii < argc; ++ii) {
        printf(" - %d: [%s]\n", ii, argv[ii]);
    }
    return 0;
}
