
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

    printf("Before\n");

    if ((cpid = fork())) {
        // we're in the parent
        int pid1 = getpid();
        int par1 = getppid();

        printf("During\n");

        int st;
        //wait(&st);
        waitpid(cpid, &st, 0);
        printf("After\n");
    }
    else {
        // we're in the child
        int pid2 = getpid();
        int par2 = getppid();

        execlp("echo", "echo", "In", "subprocess", NULL);
        // The process is still running after exec.
        // But this program isn't running in it.
        // So this print will never happen.
        printf("After exec.\n");
    }

    printf("All done!\n");

    return 0;
}
