#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>

#include "orderedSet.h"

#define LEFT (0)
#define RIGHT (1)
#define TREE_NUM_CHILDREN (2)

/* Data structure AVL tree implementation courtesy of James Aspnes, 2015. */
struct orderedSet {
    struct tree *tree;
};
struct tree {
    /* we'll make this an array so that we can make some operations symmetric */
    struct tree *child[TREE_NUM_CHILDREN];
    char *key;
    int height;    /* height of this node */
    size_t size;   /* size of subtree rooted at this node */
};

#define TREE_EMPTY (0)
#define TREE_EMPTY_HEIGHT (-1)

int
treeHeight(const struct tree *root)
{
    if(root == 0) {
        return TREE_EMPTY_HEIGHT;
    } else {
        return root->height;
    }
}

/* recompute height from height of kids */
static int
treeComputeHeight(const struct tree *root)
{
    int childHeight;
    int maxChildHeight;
    int i;

    if(root == 0) {
        return TREE_EMPTY_HEIGHT;
    } else {
        maxChildHeight = TREE_EMPTY_HEIGHT;

        for(i = 0; i < TREE_NUM_CHILDREN; i++) {
            childHeight = treeHeight(root->child[i]);
            if(childHeight > maxChildHeight) {
                maxChildHeight = childHeight;
            }
        }

        return maxChildHeight + 1;
    }
}

size_t
treeSize(const struct tree *root)
{
    if(root == 0) {
        return 0;
    } else {
        return root->size;
    }
} 

/* recompute size from size of kids */
static int
treeComputeSize(const struct tree *root)
{
    int size;
    int i;

    if(root == 0) {
        return 0;
    } else {
        size = 1;

        for(i = 0; i < TREE_NUM_CHILDREN; i++) {
            size += treeSize(root->child[i]);
        }

        return size;
    }
}

/* fix aggregate data in root */
/* assumes children are correct */
static void
treeFix(struct tree *root)
{
    if(root) {
        root->height = treeComputeHeight(root);
        root->size = treeComputeSize(root);
    }
}

/* rotate child in given direction to root */
static void
treeRotate(struct tree **root, int direction)
{
    struct tree *x;
    struct tree *y;
    struct tree *b;

    /*
     *      y           x 
     *     / \         / \
     *    x   C  <=>  A   y
     *   / \             / \
     *  A   B           B   C
     */

    y = *root;                  assert(y);
    x = y->child[direction];    assert(x);
    b = x->child[!direction];

    /* do the rotation */
    *root = x;
    x->child[!direction] = y;
    y->child[direction] = b;

    /* fix y then x */
    treeFix(y);
    treeFix(x);
}

/* restore AVL property at *root after an insertion or deletion */
static void
treeRebalance(struct tree **root)
{
    int tallerKid;

    if(*root) {
        for(tallerKid = 0; tallerKid < TREE_NUM_CHILDREN; tallerKid++) {
            if(treeHeight((*root)->child[tallerKid]) >= treeHeight((*root)->child[!tallerKid]) + 2) {

                /* check if zig-zag: opposite-direction nephew is the tall one */
                /* this also covers case where both nephews are too tall */
                if(treeHeight((*root)->child[tallerKid]->child[!tallerKid]) 
                            >= treeHeight((*root)->child[tallerKid]) - 1) {
                    /* zig zag case */
                    treeRotate(&(*root)->child[tallerKid], !tallerKid);
                }

                /* fall through to zig zig case */
                treeRotate(root, tallerKid);

                /* don't bother with other kid */
                break;
            }
        }
    }
}


/* free all elements of a tree, replacing it with TREE_EMPTY */
void 
treeDestroy(struct tree **root)
{
    int i;

    if(*root) {
        for(i = 0; i < TREE_NUM_CHILDREN; i++) {
            treeDestroy(&(*root)->child[i]);
        }
        free((*root)->key);
        free(*root);
        *root = 0;
    }
}



/* insert an element into a tree pointed to by root */
void
treeInsert(struct tree **root, const char *newElement)
{
    struct tree *e;

    if(*root == 0) {
        /* not already there, put it in */

        e = malloc(sizeof(*e));
        assert(e);
        int len = strlen(newElement);
        e->key = malloc(sizeof(char) * (len + 1));
        strcpy(e->key, newElement);

        e->child[LEFT] = e->child[RIGHT] = 0;

        *root = e;
    } else {
        /* do this recursively so we can fix data on the way back out */
        if(strcmp((*root)->key, newElement) < 0) {
            treeInsert(&(*root)->child[RIGHT], newElement);
        } else {
            treeInsert(&(*root)->child[LEFT], newElement);
        }
    }

    /* fix the aggregate data */
    treeFix(*root);
    treeRebalance(root);
}

/* return 1 if target is in tree, 0 otherwise */
int
treeContains(const struct tree *t, const char *target)
{
    while(t && (strcmp(t->key, target) != 0)) {
        if(strcmp(t->key, target) < 0) {
            t = t->child[RIGHT];
        } else {
            t = t->child[LEFT];
        }
    }

    return t != 0;  
}

/* delete minimum element from the tree and return its key */
/* do not call this on an empty tree */
char *
treeDeleteMin(struct tree **root)
{
    struct tree *toFree;
    char *retval;

    assert(*root);  /* can't delete min from empty tree */

    if((*root)->child[LEFT]) {
        /* recurse on left subtree */
        retval = treeDeleteMin(&(*root)->child[LEFT]);
    } else {
        /* delete the root */
        toFree = *root;
        retval = toFree->key;
        *root = toFree->child[RIGHT];
        free(toFree);
    }

    /* fix the aggregate data */
    treeFix(*root);
    treeRebalance(root);

    return retval;
}

/* delete target from the tree */
/* has no effect if target is not in tree */
void 
treeDelete(struct tree **root, const char *target)
{
    struct tree *toFree;

    /* do nothing if target not in tree */
    if(*root) {
        if(strcmp((*root)->key, target) == 0) {
            if((*root)->child[RIGHT]) {
                /* replace root with min value in right subtree */
                free((*root)->key);
                (*root)->key = treeDeleteMin(&(*root)->child[RIGHT]);
            } else {
                /* patch out root */
                toFree = *root;
                *root = toFree->child[LEFT];
                free(toFree->key);
                free(toFree);
            }
        } else {
            if(strcmp((*root)->key, target) < 0) {
                treeDelete(&(*root)->child[RIGHT], target);
            } else {
                treeDelete(&(*root)->child[LEFT], target);
            }
        }

        /* fix the aggregate data */
        treeFix(*root);
        treeRebalance(root);
    }
}

struct orderedSet *orderedSetCreate(void) {
    struct orderedSet *os = malloc(sizeof(struct orderedSet));
    os->tree = TREE_EMPTY;
    return os;
}

/* Destroy a set */
void orderedSetDestroy(struct orderedSet *os) {
    treeDestroy(&os->tree);
    free(os);
}

/* How many elements in this set? */
size_t orderedSetSize(const struct orderedSet *os) {
    if (!os->tree){
        return 0;
    }
    return treeComputeSize(os->tree);
}

/* Insert a new element.  Has no effect if element is already present. */
void orderedSetInsert(struct orderedSet *os, const char *str) {
    if(treeContains(os->tree, str) == 0) {
        treeInsert(&os->tree, str);
    }
}

/* Delete an element.  Has no effect if element is not already present. */
void orderedSetDelete(struct orderedSet *os, const char *str) {
    treeDelete(&os->tree, str);
}

void treeTraverseandAdd(struct tree *root, struct orderedSet *at, int (*predicate)(void *arg, const char *x), void *arg) {
    if(root->child[LEFT]) {
        treeTraverseandAdd(root->child[0], at, predicate, arg);
    }
    if(predicate(arg, root->key) != 0) {
        orderedSetInsert(at, root->key);
    }
    if(root->child[RIGHT]) {
        treeTraverseandAdd(root->child[1], at, predicate, arg);
    } 
}


/* Return a new ordered set containing all elements e
 * for which predicate(arg, x) != 0.
 * The predicate function should be applied to the elements in increasing order. */
struct orderedSet *orderedSetFilter(const struct orderedSet *os, int (*predicate)(void *arg, const char *x), void *arg) {
    struct orderedSet *addto = malloc(sizeof(struct orderedSet));
    addto->tree = TREE_EMPTY;
    if(os->tree == NULL) {
        return addto;
    }
    treeTraverseandAdd(os->tree, addto, predicate, arg);
    return addto;

}





