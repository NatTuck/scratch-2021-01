#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/mman.h>

#include "barrier.h"

barrier*
make_barrier(int nn)
{
    int rv;

    barrier* bb = mmap(
        0, sizeof(barrier), PROT_READ|PROT_WRITE,
        MAP_SHARED|MAP_ANONYMOUS, -1, 0
    );
    if ((long) bb == -1) {
        perror("mmap");
        exit(1);
    }

    rv = sem_init(&(bb->barrier), 1, 0);
    if (rv == -1) {
        perror("sem_init(1)");
        exit(1);
    }

    rv = sem_init(&(bb->mutex), 1, 1);
    if (rv == -1) {
        perror("sem_init(2)");
        exit(1);
    }

    bb->count = nn;
    bb->seen = 0;
    return bb;
}

// This barrier structure can't be reused.
// Each process must wait exactly once
// on it.

void
barrier_wait(barrier* bb)
{
    int rv;

    rv = sem_wait(&(bb->mutex));
    if (rv == -1) {
        perror("sem_wait(1)");
        exit(1);
    }

    bb->seen += 1;
    int seen = bb->seen;

    rv = sem_post(&(bb->mutex));
    if (rv == -1) {
        perror("sem_post(1)");
        exit(1);
    }

    if (seen >= bb->count) {
        for (int ii = 0; ii < bb->count; ++ii) {
            rv = sem_post(&(bb->barrier));
            if (rv == -1) {
                perror("sem_post(2)");
                exit(1);
            }
        }
    }

    rv = sem_wait(&(bb->barrier));
    if (rv == -1) {
        perror("sem_wait(2)");
        exit(1);
    }
}

void
free_barrier(barrier* bb)
{
    int rv = munmap(bb, sizeof(barrier));
    // check error?
}
