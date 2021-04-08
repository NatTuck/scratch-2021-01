#include <stdio.h>

#include "list.h"

cell*
read_list()
{
    long xx;
    if (1 == scanf("%ld", &xx)) {
        cell* rest = read_list();
        return cons(xx, rest);
    }
    else {
        return 0;
    }
}

cell*
print_list(cell* xs)
{
    while (xs) {
        printf("%ld ", car(xs));
        xs = cdr(xs);
    }
    printf("\n");
}

int
main(int argc, char* argv[])
{
    cell* xs = read_list();
    cell* ys = reverse(xs);
    print_list(xs);
    print_list(ys);
    free_list(xs);
    free_list(ys);
    return 0;
}


