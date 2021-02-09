
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct goat {
    char* name;
    float mass; // kilos
    // 4 bytes of padding
} goat;

int
main(int argc, char* argv[])
{
    printf("sizeof(goat) = %ld\n", sizeof(goat));

    goat alice;  // Alice is 0(%rsp) (size = 12B)
    alice.name = strdup("Alice");
    alice.mass = 10;

    printf(
        "%s weighs %.02f kilos\n",
        alice.name,
        alice.mass
    );

    free(alice.name);

    goat* bob = malloc(sizeof(goat));
    (*bob).name = strdup("Bob");
    bob->mass = 10;

    printf(
        "%s weighs %.02f kilos\n",
        bob->name,
        bob->mass
    );

    free(bob->name);
    free(bob);
    return 0;
}
