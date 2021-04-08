
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int
main(int _ac, char* _av[])
{
    int cpid;
    if ((cpid = fork())) {
        waitpid(cpid, 0, 0);
        printf("%d: Child done.\n", getpid());
    }
    else {
        int fd = open("/tmp/stdout.txt", O_CREAT | O_APPEND | O_WRONLY, 0644);
        close(1);
        dup(fd);
        close(fd);

        if ((cpid = fork())) {
            waitpid(cpid, 0, 0);
            printf("%d: After exec.\n", getpid());
        }
        else {
            execlp("echo", "echo", "exec'd", "echo", NULL);
        }
    }
    return 0;
}

