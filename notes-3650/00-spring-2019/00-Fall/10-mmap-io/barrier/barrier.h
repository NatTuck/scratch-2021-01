#ifndef BARRIER_H
#define BARRIER_H

#include <semaphore.h>

typedef struct barrier {
    sem_t barrier;
    sem_t mutex;
    int   count;
} barrier;

barrier* make_barrier(int nn);
void barrier_wait(barrier* bb);
void free_barrier(barrier* bb);


#endif

