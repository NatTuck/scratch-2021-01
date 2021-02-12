
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "map.h"

// calloc(a, b) is:
//  - char* p = malloc(a * b);
//  - for (long ii = 0; ii < (a*b); ++ii) {
//      p[ii] = 0;
//    }

pair*
make_pair(const char* key,
          const char* val,
          pair* next)
{
    pair* pp = malloc(sizeof(pair));
    pp->key = strdup(key); // take ownership
    pp->val = strdup(val); // of copies
    pp->next = next;
    return pp;
}

void
free_pair(pair* pp)
{
    if (pp == 0) {
        return;
    }

    free_pair(pp->next);
    free(pp->key);
    free(pp->val);
    free(pp);
}

map*
make_map()
{
    map* mm = malloc(sizeof(map));
    mm->size = 0;
    mm->capacity = 4;
    mm->data = calloc(mm->capacity, sizeof(pair*));
    return mm;
}

void
free_map(map* mm)
{
    for (long ii = 0; ii < mm->capacity; ++ii) {
        free_pair(mm->data[ii]);
    }
    free(mm->data);
    free(mm);
}

long
hash(const char* text, long nn)
{
    // really hash + modulo
    long hh = 0;
    for (long ii = 0; text[ii]; ++ii) {
        hh = hh * 67 + text[ii];
    }
    return hh & (nn - 1);
}

void
map_grow(map* mm)
{
    long nn = mm->capacity;
    pair** data = mm->data;

    mm->capacity = 2 * nn;
    mm->data = calloc(mm->capacity, sizeof(pair*));
    mm->size = 0;

    for (long ii = 0; ii < nn; ++ii) {
        for (pair* cc = data[ii]; cc; cc = cc->next) {
            map_put(mm, cc->key, cc->val);
        }
        free_pair(data[ii]);
    }
    free(data);
}

void
map_put(map* mm, const char* key, const char* val)
{
    if (mm->size >= mm->capacity) {
        map_grow(mm);
    }

    long ii = hash(key, mm->capacity);
    mm->data[ii] = make_pair(key, val, mm->data[ii]);
    mm->size += 1;
}

char*
map_get(map* mm, const char* key)
{
    long ii = hash(key, mm->capacity);
    for (pair* cc = mm->data[ii]; cc; cc = cc->next) {
        if (strcmp(cc->key, key) == 0) {
            return cc->val;
        }
    }
    return 0;
}

void
alist_del(pair** ref, const char* key)
{
    pair* curr = *ref;
    if (!curr) {
        return;
    }

    if (strcmp(curr->key, key) == 0) {
        *ref = curr->next;
        curr->next = 0;
        free_pair(curr);
    }
    else {
        alist_del(&(curr->next), key);
    }
}

void
map_del(map* mm, const char* key)
{
    long ii = hash(key, mm->capacity);
    alist_del(&(mm->data[ii]), key);
}

void
map_dump(map* mm)
{
    for (long ii = 0; ii < mm->capacity; ++ii) {
        for (pair* cc = mm->data[ii]; cc; cc = cc->next) {
            printf(
                "%ld: %s => %s\n",
                ii,
                cc->key,
                cc->val
            );
        }
    }
}

map_it
iterate(map* mm)
{
    map_it it;
    it.mm = mm;
    it.ii = 0;
    it.item = mm->data[0];
    return it;
}

int
has_next(map_it it)
{
    it = next(it);
    return it.item != 0;
}

map_it
next(map_it it)
{
    if (it.item) {
        it.item = it.item->next;
    }

    while (it.item == 0) {
        it.ii += 1;
        //printf("looking at row %ld\n", it.ii);
        if (it.ii < it.mm->capacity) {
            it.item = it.mm->data[it.ii];
        }
        else {
            it.item = 0;
            break;
        }
    }

    return it;
}
