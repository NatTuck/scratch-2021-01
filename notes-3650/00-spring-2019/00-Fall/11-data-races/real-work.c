#include <stdio.h>
#include <assert.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <semaphore.h>

void*
malloc_shared(size_t size)
{
    return mmap(0, size, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
}

void
free_shared(void* ptr, size_t size)
{
    munmap(ptr, size);
}

int
main(int _ac, char* _av[])
{
    sem_t* locks = malloc_shared(2 * sizeof(sem_t));
    sem_t* aa = &(locks[0]);
    sem_t* bb = &(locks[1]);

    sem_init(aa, 1, 1);
    sem_init(bb, 1, 1);

    int cpid;
    if ((cpid = fork())) {
        printf("In parent\n");

        sem_wait(aa);
        sleep(1);
        sem_wait(bb);

        printf("Doing real work in parent...");

        sem_post(bb);
        sem_post(aa);

        waitpid(cpid, 0, 0);
    }
    else {
        printf("In child\n");

        sem_wait(bb);
        sleep(1);
        sem_wait(aa);

        printf("Doing real work in child...\n");

        sem_post(bb);
        sem_post(aa);
    }

    free_shared(locks, sizeof(long));
    return 0;
}


