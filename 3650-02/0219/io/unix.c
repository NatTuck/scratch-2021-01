#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int
main(int _ac, char* _av[])
{
    int thing1 = open(
        "/tmp/other.txt",
        O_WRONLY | O_CREAT | O_TRUNC,
        0644
    );
    char* msg = "Hello, Thing1\n";
    write(thing1, msg, strlen(msg));
    close(thing1);

    char temp[80];
    int thing2 = open("/tmp/other.txt", O_RDONLY);
    long rv = read(thing2, temp, 80);
    close(thing2);

    write(1, temp, rv);
    return 0;
}
