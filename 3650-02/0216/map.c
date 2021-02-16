
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

#include "map.h"

size_t
node_size(node* nn)
{
    if (nn) {
        return nn->size;
    }
    else {
        return 0;
    }
}

void
update_node_size(node* nn)
{
    if (!nn) {
        return;
    }

    size_t sl = node_size(nn->left);
    size_t sr = node_size(nn->right);
    nn->size = sl + sr + 1;
}

node*
make_node(const char* key, const char* val)
{
    node* nn = calloc(1, sizeof(node));
    nn->key = strdup(key);
    nn->val = strdup(val);
    nn->size = 1;
    return nn;
}

void
free_node(node* nn)
{
    if (!nn) {
        return;
    }

    free_node(nn->left);
    free_node(nn->right);
    free(nn->key);
    free(nn->val);
    free(nn);
}

node*
rotate_right(node* nn)
{
    if (!nn || !nn->left) {
        return nn;
    }

    node* root = nn->left;
    // root->key < nn->key
    // nn->key goes in the right child
    nn->left = root->right;
    root->right = nn;
    update_node_size(nn);
    update_node_size(root);
}

node*
rotate_left(node* nn)
{
    if (!nn || !nn->right) {
        return nn;
    }

    node* root = nn->right;
    nn->right = root->left;
    root->left = nn;
    update_node_size(nn);
    update_node_size(root);
}

node*
node_rebalance(node* nn)
{
    if (nn == 0) {
        return 0;
    }

    long sl = (long) node_size(nn->left);
    long sr = (long) node_size(nn->right);
    // size_t is unsigned long

    if ((sl - sr) > 1) {
        // too heavy on the left
        return rotate_right(nn);
    }
    else if ((sr - sl) > 1) {
        // too heavy on right
        return rotate_left(nn);
    }
    else {
        return nn;
    }
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
        // new key is less than the current
        // node's key, therefore it goes in
        // the left subtree
        nn->left = node_put(nn->left, key, val);
    }
    else {
        // new key is greater than the current
        // node's key, therefore it goes in
        // the right subtree
        nn->right = node_put(nn->right, key, val);
    }

    update_node_size(nn);
    return node_rebalance(nn);
}

char*
node_get(node* nn, const char* key)
{
    if (nn == 0) {
        return 0;
    }

    int cmp = strcmp(key, nn->key);

    if (cmp == 0) {
        // Unowned pointer
        return nn->val;
    }

    if (cmp < 0) {
        // let's check left
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
        // TODO: Actually remove node.
        free(nn->val);
        nn->val = 0;
    }
    else if(cmp < 0) {
        nn->left = node_del(nn->left, key);
    }
    else {
        nn->right = node_del(nn->right, key);
    }

    // TODO: Rebalance

    return nn;
}

void
node_dump1(node* nn, int dd)
{
    if (!nn) {
        return;
    }

    node_dump1(nn->left, dd+1);

    for (int ii = 0; ii < dd; ++ii) {
        printf(" ");
    }

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

void
map_put(map* mm, const char* key, const char* val)
{
    mm->root = node_put(mm->root, key, val);
}

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
