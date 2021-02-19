
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
        // we're in the parent
        int pid1 = getpid();
        int par1 = getppid();

        printf("Hallo, I'm %d, child of %d.\n", pid1, par1);

        int st;
        //wait(&st);
        waitpid(cpid, &st, 0);
        printf("I found the number %d\n", st);
    }
    else {
        // we're in the child
        int pid2 = getpid();
        int par2 = getppid();

        printf("Hoy, I'm %d, child of %d.\n", pid2, par2);
    }

    printf("All done!\n");

    return 0;
}
