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

    printf("Hi, I'm %d, child of %d.\n", opid, opar);

    if ((cpid = fork())) {
        int pid1 = getpid();
        int par1 = getppid();

        printf("Halo, I'm %d, child of %d.\n", pid1, par1);
        printf("I have a child: %d\n", cpid);

        int status;
        wait(&status);
        printf("Child exit status was %d\n", status);
    }
    else {
        int pid2 = getpid();
        int par2 = getppid();

        printf("Hoy, I'm %d, child %d\n", pid2, par2);
        printf("I see cpid as %d\n", cpid);
    }

    printf("All done\n");

    return 0;
}
