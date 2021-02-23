#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

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
    int rv, cpid;

    int pipes[2];
    rv = pipe(pipes);
    check_rv(rv);

    if ((cpid = fork())) {
        check_rv(cpid);
        close(pipes[1]);
    }
    else {
        close(pipes[0]);
        close(1);

        rv = dup(pipes[1]);
        check_rv(rv);

        // this is an argv array
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
        assert(rv != -1);
    } while (nn != 0);

    wait(0);
    return 0;
}
