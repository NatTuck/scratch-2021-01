#include <pthread.h>
#include <semaphore.h>
#include <stdatomic.h>
#include <assert.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <unistd.h>

int nn;

int
main(int _ac, char* _av[])
{
    int cpid;

    if ((cpid = fork())) {
        nn = 25;
        printf("%p: %d\n", &nn, nn);

        sleep(2);
        
        printf("%p: %d\n", &nn, nn);

        waitpid(cpid, 0, 0);
    }
    else {
        sleep(1);
        nn = 31;
        printf("%p: %d\n", &nn, nn);
    }

    return 0;
}
