
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
        printf("During\n");

        int st;
        waitpid(cpid, &st, 0);

        printf("After\n");
    }
    else {
        execlp("echo", "echo", "In", "subprocess", NULL);
        printf("Never get here.\n");
    }

    return 0;
}
