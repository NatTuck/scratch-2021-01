#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int
main(int _ac, char* _av[])
{
    int thing1 = open("/tmp/thing1.txt", O_CREAT | O_TRUNC | O_WRONLY);
    char* msg = "Hello, Thing1\n";
    write(thing1, msg, strlen(msg));
    close(thing1);

    char temp[80];
    int input1 = open("/tmp/thing1.txt", O_RDONLY);
    int len = read(input1, temp, 80);
    close(input1);
    temp[len] = 0;

    write(1, temp, strlen(temp));

    return 0;
}
