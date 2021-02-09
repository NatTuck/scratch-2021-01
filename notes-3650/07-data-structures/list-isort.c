// compile this file with:
//     gcc -o isort isort.c
//
// run with
//    ./isort

#include <stdio.h>
#include <stdlib.h>

// An empty list is a null pointer.
typedef struct int_list {
    int head;
    struct int_list* tail;
} int_list;

int_list*
cons(int xx, int_list* xs)
{
    int_list* ys = malloc(sizeof(int_list));
    ys->head = xx;
    ys->tail = xs;
    return ys;
}

void
free_list(int_list* xs)
{
    if (xs) {
        free_list(xs->tail);
        free(xs);
    }
}

// leave this out in first pass
int_list*
reverse(int_list* xs)
{
    int_list* ys = 0;
    for (; xs; xs = xs->tail) {
        ys = cons(xs->head, ys);
    }
    return ys;
}

int_list*
read_ints(int nn)
{
    int_list* ys = 0;
    int y;

    for (int ii = 0; ii < nn; ++ii) {
        scanf("%d", &y); // read int
        ys = cons(y, ys);
    }

    int_list* zs = reverse(ys);
    free_list(ys);
    return zs;
}

void
print_list(int_list* xs)
{
    for (int_list* curr = xs; curr; curr = curr->tail) {
        printf("%d ", curr->head);
    }
    printf("\n");
}

// leave this out in first pass and valgrind
int_list*
copy_list(int_list* xs)
{
    if (xs) {
        return cons(xs->head, copy_list(xs->tail));
    }
    else {
        return 0;
    }
}

// insert a number into a sorted list
int_list*
insert(int_list* xs, int xx) {
    if (xs == 0 || xs->head > xx) {
        return cons(xx, copy_list(xs));
    }
    else {
        return cons(xs->head, insert(xs->tail, xx));
    }
}

int_list*
isort(int_list* xs) {
    int_list* ys = 0;

    for (int_list* curr = xs; curr; curr = curr->tail) {
        int_list* zs = insert(ys, curr->head);
        free_list(ys);
        ys = zs;
    }

    return ys;
}


int
main(int _ac, char* _av[])
{
    int nn;

    printf("How many ints?\n");
    scanf("%d", &nn);

    printf("Enter that many ints, one per line\n");
    int_list* xs = read_ints(nn);

    int_list* ys = isort(xs);

    printf("Your array:\n");
    print_list(xs);

    printf("Sorted array:\n");
    print_list(ys);

    free_list(xs);
    free_list(ys);

    return 0;
}
