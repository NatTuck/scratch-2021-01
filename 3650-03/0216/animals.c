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
    map_put(noises, "goat", "meow");
    map_put(noises, "cheetah", "chirp");
    map_put(noises, "man", "lizard");
    map_put(noises, "dolphin", "eeeeeee");
    map_put(noises, "other man", "why segfault?");
    map_put(noises, "aardvark", "G R E E T I N G S");
    map_put(noises, "duck", "got any grapes?");

    map_put(noises, "", "");

    map_dump(noises);

    printf("The cow says %s\n", map_get(noises, "cow"));

    map_del(noises, "cow");

    map_dump(noises);

    printf("The cow says %s\n", map_get(noises, "cow"));

    /*
    for (map_it it = iterate(noises);
         it.item;
         it = next(it)) {
        printf(
            "The %s says %s\n",
            it.item->key,
            it.item->val
        );
    }
    */

    free_map(noises);
    return 0;
}
