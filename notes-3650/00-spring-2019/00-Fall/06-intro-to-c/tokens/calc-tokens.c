
#include <stdio.h>
#include <string.h>
#include <ctype.h>


typedef int (*cpred)(int);

void
scan_chars(char* tokn, char* text, cpred pred)
{
    int ii;
    for (ii = 0; pred(text[ii]); ++ii) {
        tokn[ii] = text[ii];
    }
    tokn[ii] = 0;
}

void
next_token(char* tokn, char* text)
{
    if (isdigit(text[0])) {
        scan_chars(tokn, text, isdigit);
    }
    else if (ispunct(text[0])) {
        scan_chars(tokn, text, ispunct);
    }
    else {
        tokn[0] = 0;
    }
}

int
main(int _ac, char* _av[])
{
    char line[80];
    char tokn[80];

    while (fgets(line, 80, stdin)) {
        printf("[%s]\n", line);

        int nn = strlen(line);
        int ii = 0;
        while (ii < nn) {
            next_token(tokn, line + ii);
            printf(" - '%s'\n", tokn);
            ii += strlen(tokn);

            while (!(isdigit(line[ii]) || ispunct(line[ii]))) {
                ++ii;
            }
        }
    }

    return 0;
}

