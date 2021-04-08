/* This file is lecture notes from CS 3650, Fall 2018 */
/* Author: Nat Tuck */

#include <stdio.h>
#include <stdlib.h>

typedef struct cell {
    int head;
    struct cell* tail;
} cell;

cell*
make_cell(int item, cell* rest)
{
    cell* xs = malloc(sizeof(cell));
    xs->head = item;
    xs->tail = rest;
    return xs;
}

void
free_cell(cell* xs)
{
    if (xs->tail) {
        free_cell(xs->tail);
    }

    free(xs);
}

cell*
take(cell* xs, int kk)
{
    if (kk <= 0) {
        return 0;
    }
    else {
        return make_cell(xs->head, take(xs->tail, kk - 1));
    }
}

cell*
drop(cell* xs, int kk)
{
    if (kk <= 0) {
        return xs;
    }
    else {
        return drop(xs->tail, kk - 1);
    }
}

void
print_list(cell* xs)
{
    if (xs == 0) {
        printf("\n");
    }
    else {
        printf("%d ", xs->head);
        print_list(xs->tail);
    }
}

int
main(int _ac, char* av[])
{
    cell* xs = 0;
    for (int ii = 0; ii < 10; ++ii) {
        xs = make_cell(10*ii, xs);
    }

    puts("Whole list:");
    print_list(xs);

    puts("First half:");
    print_list(take(xs, 5));

    puts("Second half:");
    print_list(drop(xs, 5));

    free_cell(xs);
    return 0;
}
