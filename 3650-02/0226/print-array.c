#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

void
assert_ok(long rv, const char* call)
{
    if (rv == -1) {
        perror(call);
        exit(1);
    }
}

int
main(int _ac, char* _av[])
{
    int size = 10 * sizeof(int);

    int fd = open(
        "./array.mem",
        O_RDWR
    );
    assert_ok(fd, "open");

    int* xs = mmap(
        0, // don't care where in memory
        size, // 40 < 4096, so we get 4096
        PROT_READ|PROT_WRITE, // but not executable
        MAP_SHARED, // other processes can see our changes
        fd, // map in the file we opened
        0   // offset = 0
    );
    assert_ok((long)xs, "mmap");

    for (int ii = 0; ii < 10; ++ii) {
        printf("%d: %d\n", ii, xs[ii]);
    }

    int rv = munmap(xs, size);
    assert_ok(rv, "munmap");

    rv = close(fd);
    assert_ok(rv, "close");

    return 0;
}
