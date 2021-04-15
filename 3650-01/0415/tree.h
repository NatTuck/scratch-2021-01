#ifndef TREE_H
#define TREE_H

typedef struct tree {
    long key;
    long val;
    struct tree* kids[2];
    long refs;
    long rank; // tree height
} tree;

tree* tree_new();
tree* tree_inc_ref(tree* tt);
tree* tree_dec_ref(tree* tt);
tree* tree_put(tree* tt, long kk, long vv);
long  tree_get(tree* tt, long kk);
char* tree_dump(tree* tt);


#endif
