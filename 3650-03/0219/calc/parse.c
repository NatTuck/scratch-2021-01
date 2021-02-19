
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "ast.h"
#include "parse.h"
#include "list.h"

int
streq(const char* aa, const char* bb)
{
    return strcmp(aa, bb) == 0;
}

int
find_first_index(list* toks, const char* tt)
{
    int ii = 0;
    for (list* it = toks; it; it = it->tail) {
        if (streq(it->head, tt)) {
            return ii;
        }
        ii++;
    }

    return -1;
}

int
contains(list* toks, const char* tt)
{
    return find_first_index(toks, tt) >= 0;
}

list*
slice(list* xs, int i0, int i1)
{
    list* ys = 0;
    list* it = xs;
    for (int ii = 0; ii < i0; ++ii) {
        it = it->tail;
    }
    for (int ii = i0; ii < i1; ++ii) {
        ys = cons(it->head, ys);
        it = it->tail;
    }
    return rev_free(ys);
}

calc_ast*
parse(list* toks)
{
    if (length(toks) == 1) {
        int vv = atoi(toks->head);
        return make_ast_value(vv);
    }

    const char* ops[] = {"+", "-", "*", "/"};

    for (int ii = 0; ii < 4; ++ii) {
        const char* op = ops[ii];

        if (contains(toks, op)) {
            int jj = find_first_index(toks, op);
            list* xs = slice(toks, 0, jj);
            list* ys = slice(toks, jj + 1, length(toks));
            calc_ast* ast = make_ast_op(op[0], parse(xs), parse(ys));
            free_list(xs);
            free_list(ys);
            return ast;
        }
    }

    fprintf(stderr, "Invalid token stream");
    exit(1);
}

