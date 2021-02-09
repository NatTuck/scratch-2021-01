
#include <stdio.h>
#include <stdlib.h>

typedef struct goat {
    char* name;
    float mass; // kg
} goat;

int
main(int _ac, char* _av[])
{
    goat* bob = malloc(sizeof(goat));
    (*bob).name = "Bob";
    bob->mass = 10;

    printf("%s has a mass of %.01f kg\n", bob->name, bob->mass);

    goat alice;
    alice.name = "Alice";
    alice.mass = 10;

    printf("%s has a mass of %.01f kg\n", alice.name, alice.mass);

    return 0;
}
