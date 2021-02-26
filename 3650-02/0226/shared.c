#include <pthread.h>
#include <semaphore.h>
#include <stdatomic.h>
#include <assert.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <unistd.h>

int
main(int _ac, char* _av[])
{
    int rv, cpid;

    int* shared = mmap(
        0, 4096, PROT_READ|PROT_WRITE,
        MAP_SHARED|MAP_ANONYMOUS, -1, 0
    );

    for (int ii = 0; ii < 10; ++ii) {
        shared[ii] = ii;
    }

    if ((cpid = fork())) {
        // parent code
        printf("Parent, sleep 1\n");
        //sleep(1);

        printf("parent: mutating array\n");
        for (int ii = 0; ii < 10; ++ii) {
            shared[ii] = 100*ii;
        }

        waitpid(cpid, 0, 0);
    }
    else {
        // child code
        printf("child, array contents\n");
        for (int ii = 0; ii < 10; ++ii) {
            printf("%d ", shared[ii]);
        }
        printf("\n");

        printf("child: sleep 2\n");
        //sleep(2);

        printf("child, array contents\n");
        for (int ii = 0; ii < 10; ++ii) {
            printf("%d ", shared[ii]);
        }
        printf("\n");
    }

    munmap(shared, 4096);
    return 0;
}
