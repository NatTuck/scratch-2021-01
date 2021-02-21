
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "tokens.h"
#include "list.h"

// '\0': the value zero, type of char implied
// NULL: the value zero, type of void* implied
// 0: the value zero, nothing about type implied
//  - could be a signed or unsigned int
//  - any kind of pointer
//  - could be char
//
// void* goat = '\0'; // this should be warning

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
tokenize(char* text)
{
    list* xs = 0;

    // " 2 + 35*9"
    //       ^

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

        // Else, operator
        char op[2] = "x"; // "x" is {'x', 0}
        op[0] = text[ii];
        xs = cons(op, xs);
        ++ii;
    }

    return rev_free(xs);
}
