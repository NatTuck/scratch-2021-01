
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

int
main(int _ac, char* _av[])
{
    int opid = getpid();
    int opar = getppid();
    int cpid;

    printf("Hi, I'm %d, child of %d\n", opid, opar);

    if ((cpid = fork())) {
        int pid1 = getpid();
        int par1 = getppid();
        printf("Hallo, I'm %d, child of %d\n", pid1, par1);

        int st;
        wait(&st);
        printf("I found the number %d\n", st);
    }
    else {
        int pid2 = getpid();
        int par2 = getppid();
        printf("Ahoy! I'm %d, child of %d\n", pid2, par2);
    }

    return 0;
}
