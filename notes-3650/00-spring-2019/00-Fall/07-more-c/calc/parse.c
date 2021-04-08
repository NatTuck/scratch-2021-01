
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "ast.h"
#include "parse.h"
#include "svec.h"

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

svec*
tokenize(const char* text)
{
    svec* xs = make_svec();

    int nn = strlen(text);
    int ii = 0;
    while (ii < nn) {
        if (isspace(text[ii])) {
            ++ii;
            continue;
        }

        if (isdigit(text[ii])) {
            char* num = read_number(text, ii);
            svec_push_back(xs, num);
            ii += strlen(num);
            free(num);
            continue;
        }

        // Else, operator.
        char op[4] = "x";
        op[0] = text[ii];
        svec_push_back(xs, op);
        ++ii;
    }

    return xs;
}

int
streq(const char* aa, const char* bb)
{
    return strcmp(aa, bb) == 0;
}

int
find_first_index(svec* toks, const char* tt)
{
    for (int ii = 0; ii < toks->size; ++ii) {
        if (streq(toks->data[ii], tt)) {
            return ii;
        }
    }

    return -1;
}

int
contains(svec* toks, const char* tt)
{
    return find_first_index(toks, tt) >= 0;
}

svec*
slice(svec* xs, int i0, int i1)
{
    svec* ys = make_svec();
    for (int ii = i0; ii < i1; ++ii) {
        svec_push_back(ys, xs->data[ii]);
    }
    return ys;
}

calc_ast*
parse(svec* toks)
{
    if (toks->size == 1) {
        int vv = atoi(toks->data[0]);
        return make_ast_value(vv);
    }

    const char* ops[] = {"+", "-", "*", "/"};

    for (int ii = 0; ii < 4; ++ii) {
        const char* op = ops[ii];

        if (contains(toks, op)) {
            int jj = find_first_index(toks, op);
            svec* xs = slice(toks, 0, jj);
            svec* ys = slice(toks, jj + 1, toks->size);
            calc_ast* ast = make_ast_op(op[0], parse(xs), parse(ys));
            free_svec(xs);
            free_svec(ys);
            return ast;
        }
    }

    fprintf(stderr, "Invalid token stream");
    exit(1);
}

calc_ast*
calc_parse(char* text)
{
    svec* toks = tokenize(text);

    /*
    printf("== tokens:\n");
    for (int ii = 0; ii < toks->size; ++ii) {
        printf("[%s]\n", toks->data[ii]);
    }
    */

    calc_ast* ast = parse(toks);
    free_svec(toks);
    return ast;
}

