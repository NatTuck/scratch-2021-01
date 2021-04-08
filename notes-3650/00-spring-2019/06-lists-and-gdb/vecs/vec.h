#ifndef VEC_H
#define VEC_H

typedef struct vec {
    long* data;
    long size;
    long cap;
} vec;

vec* make_vec();
void free_vec(vec* xs);
void push(vec* xs, long xx);
vec* reverse(vec* xs);

#endif
