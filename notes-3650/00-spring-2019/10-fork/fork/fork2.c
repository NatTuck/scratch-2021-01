
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

    printf("init: Hi, I'm %d, child of %d\n", opid, opar);

    if ((cpid = fork())) {
        int pid1 = getpid();
        int par1 = getppid();
        printf("parent: Hallo, I'm %d, child of %d\n", pid1, par1);
        printf("parent: The new child is %d\n", cpid);

        int st;
        wait(&st);
    }
    else {
        int pid2 = getpid();
        int par2 = getppid();
        printf("child: Ahoy! I'm %d, child of %d\n", pid2, par2);
    }

    return 0;
}
