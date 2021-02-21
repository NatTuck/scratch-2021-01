#include <stdio.h>

int
main(int _ac, char* _av[])
{
    FILE* thing1 = fopen("/tmp/avacado.txt", "w");
    fprintf(thing1, "Hello, Thing1\n");
    fclose(thing1);

    char temp[80];
    FILE* thing2 = fopen("/tmp/avacado.txt", "r");
    fgets(temp, 80, thing2);
    fclose(thing2);

    fprintf(stdout, "%s", temp);

    return 0;
}
