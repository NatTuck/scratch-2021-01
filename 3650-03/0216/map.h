#ifndef MAP_H
#define MAP_H

typedef struct node {
    char* key;
    char* val;
    struct node* left;
    struct node* right;
    size_t size; // how big is this subtree
} node;

typedef struct map {
    node* root;
} map;

map* make_map();
void free_map();
void map_put(map* mm, const char* key, const char* val);
char* map_get(map* mm, const char* key);
void map_del(map* mm, const char* key);
void map_dump(map* mm);

#endif
