/* This file is lecture notes from CS 3650, Fall 2018 */
/* Author: Nat Tuck */

#include <stdio.h>
#include <stdlib.h>

typedef struct cell {
    int head;
    struct cell* tail;
    int refs;
} cell;

cell*
make_cell(int item, cell* rest)
{
    cell* xs = malloc(sizeof(cell));
    xs->head = item;
    xs->tail = rest;
    xs->refs = 1;
    return xs;
}

cell*
dup_cell(cell* xs)
{
    xs->refs += 1;
    return xs;
}

void
free_cell(cell* xs)
{
    xs->refs -= 1;
    if (xs->refs > 0) {
        return;
    }
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
        return dup_cell(xs);
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
    cell* xs1 = take(xs, 5);
    print_list(xs1);
    free_cell(xs1);

    puts("Second half:");
    cell* xs2 = drop(xs, 5);
    print_list(xs2);
    free_cell(xs2);

    free_cell(xs);
    return 0;
}
