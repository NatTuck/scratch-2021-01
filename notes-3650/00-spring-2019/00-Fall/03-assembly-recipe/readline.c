#include <stdio.h>

void
readline(char* buf, long len)
{
    fgets(buf, len, stdin);
}

