#include <stdio.h>

#include "map.h"

int
main(int _ac, char* _av[])
{
    map* noises = make_map();
    map_put(noises, "chicken", "cluck");
    map_put(noises, "cow", "moo");
    map_put(noises, "elephant", "AAAAA");
    map_put(noises, "dog", "bork");
    map_put(noises, "cat", "woof");

    printf("The cow says %s\n", map_get(noises, "cow"));

    map_del(noises, "cow");

    map_dump(noises);

    for (map_it it = iterate(noises);
         it.item;
         it = next(it)) {
        printf(
            "The %s says %s\n",
            it.item->key,
            it.item->val
        );
    }

    free_map(noises);
    return 0;
}
