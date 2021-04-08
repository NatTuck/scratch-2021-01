#include <stdio.h>

int
main(int _ac, char* _av[])
{
    FILE* thing1 = fopen("/tmp/thing1.txt", "w");
    fprintf(thing1, "Hello, Thing1\n");
    fclose(thing1);

    char temp[80];
    FILE* input1 = fopen("/tmp/thing1.txt", "r");
    fgets(temp, 80, input1);
    fclose(input1);

    fprintf(stdout, "%s", temp);

    return 0;
}
