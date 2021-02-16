#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "map.h"

node*
make_node(const char* key, const char* val)
{
    node* nn = calloc(1, sizeof(node));
    nn->key = strdup(key);
    nn->val = strdup(val);
    return nn;
}

void
free_node(node* nn)
{
    if (nn == 0) {
        return;
    }

    free_node(nn->left);
    free_node(nn->right);
    free(nn->key);
    free(nn->val);
}

node*
node_put(node* nn, const char* key, const char* val)
{
    if (nn == 0) {
        return make_node(key, val);
    }

    int cmp = strcmp(key, nn->key);

    if (cmp == 0) {
        free(nn->val);
        nn->val = strdup(val);
        return nn;
    }

    if (cmp < 0) {
        nn->left = node_put(nn->left, key, val);
    }
    else {
        nn->right = node_put(nn->right, key, val);
    }

    // TODO: Rebalance

    return nn;
}

char*
node_get(node* nn, const char* key)
{
    if (nn == 0) {
        return 0;
    }

    int cmp = strcmp(key, nn->key);

    if (cmp == 0) {
        return nn->val;
    }

    if (cmp < 0) {
        return node_get(nn->left, key);
    }
    else {
        return node_get(nn->right, key);
    }
}

node*
node_del(node* nn, const char* key)
{
    if (nn == 0) {
        return 0;
    }

    int cmp = strcmp(key, nn->key);

    if (cmp == 0) {
        free(nn->val);
        nn->val = 0;
    }
    else if (cmp < 0) {
        nn->left = node_del(nn->left, key);
   }
    else {
        nn->right = node_del(nn->right, key);
    }

    // TODO: Rebalance

    return nn;
}

void
indent(int dd)
{
    for (int ii = 0; ii < dd; ++ii) {
        printf(" ");
    }
}

void
node_dump1(node* nn, int dd)
{
    if (!nn) {
        return;
    }

    node_dump1(nn->left, dd+1);

    indent(dd);
    printf("%s => %s\n", nn->key, nn->val);

    node_dump1(nn->right, dd+1);
}

void
node_dump(node* nn)
{
    if (nn) {
        node_dump1(nn, 0);
    }
    else {
        puts("(empty)");
    }
}

map*
make_map()
{
    return calloc(1, sizeof(map));
}

void
free_map(map* mm)
{
    free_node(mm->root);
    free(mm);
}

// put: map makes owned copies of key, val
void
map_put(map* mm, const char* key, const char* val)
{
    mm->root = node_put(mm->root, key, val);
}

// get: returns an unowned reference to val
char*
map_get(map* mm, const char* key)
{
    return node_get(mm->root, key);
}

void
map_del(map* mm, const char* key)
{
    mm->root = node_del(mm->root, key);
}

void
map_dump(map* mm)
{
    node_dump(mm->root);
}
