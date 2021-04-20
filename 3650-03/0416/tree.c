
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdatomic.h>

#include "tree.h"

/*
typedef struct tree {
    long key;
    long val;
    struct tree* kids[2];
    long refs;
    long rank; // tree height
} tree;
*/

static
long
get_rank(tree* tt)
{
    if (tt) {
        return tt->rank;
    }
    else {
        // this is important
        return -1;
    }
}

static
long
calc_rank(tree* tt)
{
    long r0 = get_rank(tt->kids[0]);
    long r1 = get_rank(tt->kids[1]);
    return 1 + ((r0 > r1) ? r0 : r1);
}

tree*
tree_new()
{
    return 0;
}

tree*
tree_inc_ref(tree* tt)
{
    if (tt) {
        atomic_fetch_add(&(tt->refs), 1);
    }
    return tt;
}

tree*
tree_dec_ref(tree* tt)
{
    if (tt) {
        long refs = atomic_fetch_sub(&(tt->refs), 1);
        if (refs <= 1) {
            tree_dec_ref(tt->kids[0]);
            tree_dec_ref(tt->kids[1]);
            free(tt);
        }
    }
    return tt;
}

static
tree*
tree_make_node(long kk, long vv)
{
    tree* tt = calloc(1, sizeof(tree));
    tt->key = kk;
    tt->val = vv;
    tt->refs = 1;
    return tt;
}

static
tree*
tree_clone(tree* tt)
{
    tree* uu = tree_make_node(tt->key, tt->val);
    uu->kids[0] = tree_inc_ref(tt->kids[0]);
    uu->kids[1] = tree_inc_ref(tt->kids[1]);
    uu->rank = calc_rank(uu);
    return uu;
}

tree*
tree_put(tree* tt, long kk, long vv)
{
    if (tt == 0) {
        return tree_make_node(kk, vv);
    }

    if (tt->key == kk) {
        tree* uu = tree_clone(tt);
        uu->val = vv;
        return uu;
    }

    tree* uu = tree_clone(tt);
    if (tt->key > kk) {
        // put in left subtree
        tree* k0 = tt->kids[0];
        uu->kids[0] = tree_put(k0, kk, vv);
        tree_dec_ref(k0);
    }
    else {
        // put in right subtree
        tree* k1 = tt->kids[1];
        uu->kids[1] = tree_put(k1, kk, vv);
        tree_dec_ref(k1);
    }
    uu->rank = calc_rank(uu);

    long r0 = get_rank(uu->kids[0]);
    long r1 = get_rank(uu->kids[1]);
    long dr = r0 - r1;

    if (dr > 1) {
        //printf("unbalanced: left subtree too deep\n");

        // Reroot at left child
        tree* l_ch = uu->kids[0];

        // New root is left child
        tree* root = tree_make_node(l_ch->key, l_ch->val);

        // New left subtree is old left->left
        root->kids[0] = tree_inc_ref(l_ch->kids[0]);

        // New right child is the old root
        root->kids[1] = tree_make_node(uu->key, uu->val);

        // New right->right subtree is the old right subtree.
        root->kids[1]->kids[1] = tree_inc_ref(uu->kids[1]);

        // New right->left is the old left->right
        tree* lrst = uu->kids[0]->kids[1];
        root->kids[1]->kids[0] = tree_inc_ref(lrst);

        tree_dec_ref(uu);
        return root;
    }

    if (dr < -1) {
        printf("unbalanced: right subtree too deep\n");

        tree* r_ch = uu->kids[1];
        tree* root = tree_make_node(r_ch->key, r_ch->val);
        root->kids[1] = tree_inc_ref(r_ch->kids[1]);
        root->kids[0] = tree_make_node(uu->key, uu->val);
        root->kids[0]->kids[0] = tree_inc_ref(uu->kids[0]);
        tree* rlst = uu->kids[1]->kids[0];
        root->kids[0]->kids[1] = tree_inc_ref(rlst);

        tree_dec_ref(uu);
        return root;
    }

    return uu;
}

long
tree_get(tree* tt, long kk)
{
    if (tt == 0) {
        return 0;
    }

    if (tt->key == kk) {
        return tt->val;
    }

    if (tt->key > kk) {
        return tree_get(tt->kids[0], kk);
    }
    else {
        return tree_get(tt->kids[1], kk);
    }
}

char*
tree_dump(tree* tt)
{
    if (tt == 0) {
        return strdup(".");
    }

    char* k0 = tree_dump(tt->kids[0]);
    char* k1 = tree_dump(tt->kids[1]);

    int nn = snprintf(
        0,
        0,
        "[%s (%ld => %ld) %s]",
        k0,
        tt->key,
        tt->val,
        k1
    );
    char* text = malloc(nn + 2);
    snprintf(
        text,
        nn + 1,
        "[%s (%ld => %ld) %s]",
        k0,
        tt->key,
        tt->val,
        k1
    );

    free(k0);
    free(k1);

    return text;
}
