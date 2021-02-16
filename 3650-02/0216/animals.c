
#include <stdio.h>

#include "map.h"

int
main(int _ac, char* _av[])
{
    map* noises = make_map();
    map_put(noises, "chicken", "cluck");
    map_put(noises, "cow", "bark");
    map_put(noises, "elephant", "awoo");
    map_put(noises, "dog", "owa owa");
    map_put(noises, "cat", "hello");

    printf("The cow says: %s\n", map_get(noises, "cow"));

    map_dump(noises);

    map_del(noises, "cow");
    printf("The cow says: %s\n", map_get(noises, "cow"));

    map_dump(noises);

    /*
    for (map_it it = iterate(noises);
                     has_next(it);
                     it = next(it)) {
        printf(
            "(%ld) The %s says: %s\n",
            it.ii,
            it.item->key,
            it.item->val
        );
    }
    */

    free_map(noises);
    return 0;
}
