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

        wait(0);

        printf("After\n");
    }
    else {
        // runs the "echo" program
        // passing in argv:
        // ["echo", "in", "subprocess"]
        execlp("echo", "echo", "in", "subprocess", NULL);
        // exec causes the program to be replaced
        // this print will never run
        printf("After exec\n");
    }

    printf("All done\n");

    return 0;
}
