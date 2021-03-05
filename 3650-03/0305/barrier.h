// Author: Nat Tuck
// CS3650 starter code

#ifndef BARRIER_H
#define BARRIER_H

#include <semaphore.h>

typedef struct barrier {
    sem_t barrier; // the mechanism to block processes
    sem_t mutex;   // a lock, protecting the structure
    int   count;   // the total number of processes
    int   seen;    // how many processes blocked
} barrier;

barrier* make_barrier(int nn);
void barrier_wait(barrier* bb);
void free_barrier(barrier* bb);


#endif

