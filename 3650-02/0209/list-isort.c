#include <stdio.h>
#include <stdlib.h>

// An empty list is a null ptr.
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
free_list(int_list *xs)
{
    if (xs) {
        free_list(xs->tail);
        free(xs);
    }
}

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
read_list(int nn)
{
    int_list* ys = 0;
    int y;

    for (int ii = 0; ii < nn; ++ii) {
        scanf("%d", &y);
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

int_list*
copy_list(int_list* xs)
{
    int_list* ys = reverse(xs);
    int_list* zs = reverse(ys);
    free_list(ys);
    return zs;
}

int_list*
insert(int xx, int_list* xs)
{
    if (xs == 0) {
        return cons(xx, 0);
    }

    if (xx < xs->head) {
        return cons(xx, copy_list(xs));
    }
    else {
        return cons(xs->head, insert(xx, xs->tail));
    }
}

int_list*
isort(int_list* xs)
{
    if (xs == 0) {
        return 0;
    }

    int_list* ys = isort(xs->tail);
    int_list* zs = insert(xs->head, ys);
    free_list(ys);
    return zs;
}

int
main(int _ac, char** _av)
{
    int nn;

    printf("How many?\n");
    scanf("%d", &nn);

    printf("Enter that many, one per line.\n");
    int_list* xs = read_list(nn);
    int_list* ys = isort(xs);

    printf("input list:\n");
    print_list(xs);

    printf("sorted list:\n");
    print_list(ys);

    free_list(xs);
    free_list(ys);
    return 0;
}
