
#include "barrier.h"

barrier*
make_barrier(int nn)
// nn is the number of processes that
// will wait on this barrier
{
    barrier* bb = mmap(
        0, sizeof(barrier), PROT_READ|PROT_WRITE,
        MAP_SHARED|MAP_ANONYMOUS, -1, 0
    );

    sem_init(&(bb->barrier), 1, 0);
    sem_init(&(bb->mutex), 1, 1);

    bb->count = nn; // never changes again, immutable
    bb->seen = 0;
    return bb;
}

void
barrier_wait(barrier* bb)
{
    sem_wait(&(bb->mutex));

    bb->seen += 1;
    int seen = bb->seen;

    sem_post(&(bb->mutex));

    if (seen >= bb->count) {
        for (int ii = 0; ii < bb->count; ++ii) {
            sem_post(&(bb->barrier));
        }
    }

    sem_wait(&(bb->barrier));
}

void
barrier_free()
{
    ... munmap;
}
