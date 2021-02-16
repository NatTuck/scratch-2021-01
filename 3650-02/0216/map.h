#ifndef MAP_H
#define MAP_H

typedef struct node {
    char* key;
    char* val;
    struct node* left;
    struct node* right;
    size_t size; // how many nodes in this subtree
} node;

typedef struct map {
    node* root;
} map;

/*
typedef struct map_it {
    // All unowned references
    map* mm;
    long ii;
    pair* item;
} map_it;
*/

map* make_map();
void free_map(map* mm);
void map_put(map* mm, const char* key, const char* val);
char* map_get(map* mm, const char* key);
void map_del(map* mm, const char* key);
void map_dump(map* mm);

/*
map_it iterate(map* mm);
int has_next(map_it it);
map_it next(map_it it);
*/

#endif
