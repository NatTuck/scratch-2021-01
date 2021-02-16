#ifndef MAP_H
#define MAP_H

typedef struct node {
    char* key;
    char* val;
    struct node* left;
    struct node* right;
} node;

typedef struct map {
    node* root;
} map;

map* make_map();
void free_map(map* mm);
// put: map makes owned copies of key, val
void map_put(map* mm, const char* key, const char* val);
// get: returns an unowned reference to val
char* map_get(map* mm, const char* key);
void map_del(map* mm, const char* key);
void map_dump(map* mm);

#endif
