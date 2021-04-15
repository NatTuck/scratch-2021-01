#include <stdlib.h>
#include <stdio.h>
#include <stdatomic.h>
#include <stdbool.h>

#include "tree.h"

tree* memos = 0;

long
fib(long xx)
{
    if (xx < 2) {
        return 1;
    }

    long yy = tree_get(memos, xx);
    if (yy) {
        return yy;
    }

    yy = fib(xx-1) + fib(xx-2);

    while (1) {
        tree* memos0 = memos;
        tree* memos1 = tree_put(memos0, xx, yy);
        bool done = atomic_compare_exchange_weak(
            &memos, &memos0, memos1
        );
        if (done) {
            tree_dec_ref(memos0);
            char* tmp = tree_dump(memos);
            puts(tmp);
            free(tmp);
            break;
        }
        else {
            tree_dec_ref(memos1);
        }
    }

    return yy;
}

int
main(int argc, char* argv[])
{
    if (argc != 2) {
        printf("nope\n");
        return 1;
    }

    long xx = atol(argv[1]);
    long yy = fib(xx);

    printf("fib(%ld) = %ld\n", xx, yy);

    tree_dec_ref(memos);
    return 0;
}
