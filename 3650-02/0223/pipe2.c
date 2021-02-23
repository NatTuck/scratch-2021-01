#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <assert.h>

void
check_rv(int rv)
{
    if (rv == -1) {
        perror("fail");
        exit(1);
    }
}

int
main(int _ac, char _av[])
{
    int cpid, rv;

    int pipes[2];
    rv = pipe(pipes);
    check_rv(rv);

    if ((cpid = fork())) {
        close(pipes[1]);
        check_rv(cpid);
    }
    else {
        close(pipes[0]);
        rv = close(1); // stdout
        check_rv(rv);
        rv = dup(pipes[1]);
        check_rv(rv);

        char* args[] = {"sort", "sample.txt", 0};
        execvp("sort", args);
        assert(0);
    }

    char tmp[256];
    int  nn;

    do {
        nn = read(pipes[0], tmp, 256);
        check_rv(nn);

        for (int ii = 0; ii < nn; ++ii) {
            if (tmp[ii] == 'e') {
                tmp[ii] = 'E';
            }
        }

        rv = write(1, tmp, nn);
        check_rv(rv);
    } while (nn != 0);

    wait(0); // wait at the bottom so
             // both sides of the pipe can run concurrently
    return 0;
}
