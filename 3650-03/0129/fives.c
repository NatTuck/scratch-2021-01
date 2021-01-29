#include <stdio.h>

int
all_match(char c, char* text)
{
    if (*text == 0) {
        return 1;
    }

    if (*text == c) {
        return all_match(c, text + 1);
    }

    return 0;
}

int
main(int argc, char* argv)
{
    if (all_match('5', argv[1])) {
        puts("all fives");
    }
    else {
        puts("not all fives");
    }
    return 0;
}
