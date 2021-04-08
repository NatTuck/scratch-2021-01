#ifndef LIST_H
#define LIST_H

typedef struct cell {
    long data;
    struct cell* next;
} cell;

cell* cons(long data, cell* next);
void free_list(cell* xs);
long car(cell* xs);
cell* cdr(cell* xs);
cell* reverse(cell* xs);

#endif
