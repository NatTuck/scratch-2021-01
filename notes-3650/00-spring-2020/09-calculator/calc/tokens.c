
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "ast.h"
#include "parse.h"
#include "list.h"

char*
read_number(const char* text, int ii)
{
    int nn = 0;
    while (isdigit(text[ii + nn])) {
        ++nn;
    }

    char* num = malloc(nn + 1);
    memcpy(num, text + ii, nn);
    num[nn] = 0;
    return num;
}

list*
tokenize(const char* text)
{
    list* xs = 0;

    int nn = strlen(text);
    int ii = 0;
    while (ii < nn) {
        if (isspace(text[ii])) {
            ++ii;
            continue;
        }

        if (isdigit(text[ii])) {
            char* num = read_number(text, ii);
            xs = cons(num, xs);
            ii += strlen(num);
            free(num);
            continue;
        }

        // Else, operator.
        char op[4] = "x";
        op[0] = text[ii];
        xs = cons(op, xs);
        ++ii;
    }

    return rev_free(xs);
}

