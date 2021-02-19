#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int
main(int argc, char* argv[])
{
    int thing1 = open(
        "/tmp/racoon.txt",
        O_WRONLY | O_CREAT | O_TRUNC,
        0644
        // 0644 is reasonable for regular files
        // 0755 is reasonable for directories
        //      and files you want to execute
    );
    char* msg = "Hello, trashcan\n";
    write(thing1, msg, strlen(msg));
    close(thing1);

    char temp[80];
    int thing2 = open("/tmp/racoon.txt", O_RDONLY);
    int len = read(thing2, temp, 80);
    close(thing2);
    temp[len] = 0;

    write(1, temp, len);
    return 0;
}
