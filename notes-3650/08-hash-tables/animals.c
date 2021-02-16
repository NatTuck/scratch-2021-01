
#include <stdio.h>

#include "map.h"

int
main(int _argc, char* _argv[])
{
    map* noises = make_map();
    map_put(noises, "chicken", "cluck");
    map_put(noises, "cow", "moo");
    map_put(noises, "elephant", "toot");
    map_put(noises, "dog", "woof");
    map_put(noises, "cat", "meow");

    printf("The cow says: %s\n", map_get(noises, "cow"));

    map_del(noises, "cow");

    for (map_it it = iterate(noises); has_next(it); it = next(it)) {
        printf("The %s says: %s\n", it.item->key, it.item->val);
    }

    free_map(noises);
    return 0;
}
