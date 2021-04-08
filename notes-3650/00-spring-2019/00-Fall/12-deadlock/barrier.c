// Author: Nat Tuck
// CS3650 starter code

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/mman.h>

#include "barrier.h"

barrier*
make_barrier(int nn)
{
    int rv;
    barrier* bb = mmap(0, sizeof(barrier), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if ((long) bb == -1) {
        perror("mmap(barrier)");
        abort();
    }

    rv = sem_init(&(bb->barrier), 1, 0);
    if (rv == -1) {
        perror("sem_init(barrier)");
        abort();
    }

    sem_init(&(bb->mutex), 1, 1);
    if (rv == -1) {
        perror("sem_init(mutex)");
        abort();
    }

    bb->count = nn;
    bb->seen  = 0;
    return bb;
}

void
barrier_wait(barrier* bb)
{
    int rv;

    rv = sem_wait(&(bb->mutex));
    if (rv == -1) {
        perror("sem_wait(mutex)");
        abort();
    }

    bb->seen += 1;
    int seen = bb->seen;

    rv = sem_post(&(bb->mutex));
    if (rv == -1) {
        perror("sem_post(mutex)");
        abort();
    }

    if (seen >= bb->count) {
        for (int ii = 0; ii < bb->count; ++ii) {
            rv = sem_post(&(bb->barrier));
            if (rv == -1) {
                perror("sem_post(barrier1)");
                abort();
            }
        }
    }

    rv = sem_wait(&(bb->barrier));
    if (rv == -1) {
        perror("sem_wait(barrier)");
        abort();
    }
}

void
free_barrier(barrier* bb)
{
    int rv = munmap(bb, sizeof(barrier));
    if (rv == -1) {
        perror("munmap");
        abort();
    }
}

