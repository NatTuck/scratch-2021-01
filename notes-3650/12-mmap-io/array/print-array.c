#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

// create array.mem
// save squares 1..10

void
assert_ok(long rv, char* call)
{
    if (rv == -1) {
        fprintf(stderr, "Failed call: %s\n", call);
        perror("Error:");
        exit(1);
    }
}

int
main(int _ac, char* _av[])
{
    int size = 10 * sizeof(int);

    // Talk about ORing flags.
    int fd = open("./array.mem", O_RDWR, 0644);
    assert_ok(fd, "open");

    int* xs = mmap(0, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
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
