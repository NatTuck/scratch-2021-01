#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <assert.h>

int
main(int _ac, char* _av[])
{
    int cpid;
    if ((cpid = fork())) {
        // parent
        wait(0);
        printf("%d: Child done.\n", getpid());
    }
    else {
        int fd = open(
            "stdout.txt",
            O_CREAT | O_APPEND | O_WRONLY,
            0644
        );

        close(1);
        dup(fd);
        close(fd);

        execlp("echo",   "echo", "ran", "echo", NULL);
        //  name-^  argv[0]-^
    }

    return 0;
}
