#ifndef BARRIER_H
#define BARRIER_H

// This barrier is single use.
// If we make one, we can use it once ever.
// If we need to barrier twice, we can make
// two barriers.

typedef struct barrier {
    int count; // number of processes total
    int seen;  // number waiting
    sem_t barrier;  // what waiting processes block on
    sem_t mutex; // a lock, protecting the seen field
} barrier;

barrier* make_barrier(int nn);
void barrier_wait(barrier* bb);
void free_barrier(barrier* bb);

#endif
