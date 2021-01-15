#include <pthread.h>
#include <semaphore.h>
#include <stdatomic.h>
#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>

int
main(int _ac, char* _av[])
{
    int* shared = mmap(0, 4096, PROT_READ | PROT_WRITE,
                       MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    for (int ii = 0; ii < 10; ++ii) {
        shared[ii] = ii;
    }


    int cpid;
    if ((cpid = fork())) {
        printf("parent: mutating array\n");
        for (int ii = 0; ii < 10; ++ii) {
            shared[ii] = ii * 100;
        }

        waitpid(cpid, 0, 0);
    }
    else {
        printf("child: array content:\n");
        for (int ii = 0; ii < 10; ++ii) {
            printf("%d ", shared[ii]);
        }

        printf("child: array content:\n");
        for (int ii = 0; ii < 10; ++ii) {
            printf("%d ", shared[ii]);
        }
        printf("\n");
    }

    munmap(shared, 4096);

    return 0;
}

