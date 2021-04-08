
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void
check_rv(int rv)
{
    if (rv == -1) {
        perror("fail");
        exit(1);
    }
}

int
main(int _ac, char* _av[])
{
    int rv;
    
    int pipe_fds[2];
    rv = pipe(pipe_fds);
    check_rv(rv);
  
    int p_read  = pipe_fds[0];
    int p_write = pipe_fds[1];
    int cpid;

    if ((cpid = fork())) {
        if (cpid == -1) {
            perror("fork");
            return -1;
        }

        // In the parent.
        close(p_read);
        printf("Parent pid = %d, child pid = %d\n", getpid(), cpid);

        char msg[] = "Hello, pipe.\n";

        rv = write(p_write, msg, strlen(msg));
        check_rv(rv);
    }
    else {
        // In the child
        close(p_write);
        printf("Child pid = %d, parent pid = %d\n", getpid(), getppid());

        char temp[100];
        rv = read(p_read, temp, 100);
        check_rv(rv);
        temp[rv] = 0;

        rv = write(1, temp, strlen(temp));
        check_rv(rv);
    }

    return 0;
}
