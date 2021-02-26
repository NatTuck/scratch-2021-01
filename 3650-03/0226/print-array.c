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

    int fd = open("array.mem", O_RDWR);
    assert_ok(fd, "open");

    int* xs = mmap(
        0, // let the OS pick the address
        size, // 40 < 4096 (one page), so we get 4096
        PROT_READ|PROT_WRITE, // not executable
        MAP_SHARED, // changes visible to other processes
        fd, // The file will be mapped in to the address
            // space
        0   // no offset
    );
    assert_ok((long)xs, "mmap");

    for (long ii = 0; ii < 10; ++ii) {
        printf("%d: %d\n", ii, xs[ii]);
    }

    int rv = munmap(xs, size);
    assert_ok(rv, "munmap");

    rv = close(fd);
    assert_ok(rv, "close");

    return 0;
}

// xs[1] = '\1\0\0\0'
