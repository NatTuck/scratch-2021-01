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
        int fd = open(
            "stdout.txt",
            O_CREAT | O_APPEND | O_WRONLY,
            0644
        );

        close(1); // stdout
        dup(fd); // copy fd to 1 (= stdout)
        close(fd);

        if (fork()) {
            printf("In child, to stdout\n");
        }
        else {
            execlp("echo", "echo", "ran", "echo", NULL);
        }
    }

    return 0;
}
