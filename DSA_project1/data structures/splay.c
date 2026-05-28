#include <stdio.h>
#include <stdlib.h>

//Stores a tree node with a key and pointers to left and right children
typedef struct SplayNode {
    int key;
    struct SplayNode *left;
    struct SplayNode *right;
} SplayNode;

//Represents the whole tree, storing the root and the number of elements
typedef struct SplayTree {
    SplayNode *root;
    size_t size;
} SplayTree;

//Creates and initializes a new node with the given key
static SplayNode *splayNewNode(int key) {
    SplayNode *n = malloc(sizeof(SplayNode));

    n -> key = key;
    n -> left = NULL;
    n -> right = NULL;

    return n;
}

//Performs a right rotation to move a node up in the tree
static SplayNode *splayRotateRight(SplayNode *x) {
    SplayNode *y = x -> left;

    x -> left = y -> right;
    y -> right = x;

    return y;
}

//Performs a left rotation to move a node up in the tree
static SplayNode *splayRotateLeft(SplayNode *x) {
    SplayNode *y = x -> right;

    x -> right = y -> left;
    y -> left = x;

    return y;
}

//Searches for a key and moves the accessed node closer to the root using rotations (splaying).
static SplayNode *splayNode(SplayNode *root, int key) {

    if (!root || root -> key == key) {
        return root;
    }

    if (key < root -> key) {

        if (!root -> left) {
            return root;
        }

        //Zig-Zag case: Left-Right
        if (key < root -> left -> key) {
            root -> left -> left = splayNode(root -> left -> left, key);
            root = splayRotateRight(root);
        }

        return root -> left ? splayRotateRight(root) : root;
    } else { 

        if (!root -> right)
            return root;

        //Zig-Zag case: Right-Left
        if (key > root -> right -> key) {
            root -> right -> right = splayNode(root -> right -> right, key);
            root = splayRotateLeft(root);
        }

        return root -> right ? splayRotateLeft(root) : root;
    }
}

//Recursively frees all nodes in the tree.
static void splayFreeNode(SplayNode *node) {
    if (!node) {
        return;
    }

    splayFreeNode(node -> left);
    splayFreeNode(node -> right);
    free(node);
}

//Frees the entire tree structure and its memory
void splayDestroy(SplayTree *tree) {
    if (!tree) {
        return;
    }

    splayFreeNode(tree -> root);
    free(tree);
}

//Creates and initializes an empty Splay tree
SplayTree *splayCreate() {
    SplayTree *t = malloc(sizeof(SplayTree));

    t -> root = NULL;
    t -> size = 0;

    return t;
}

//Searches for a key and moves it to the root if found
int splaySearch(SplayTree *tree, int key) {

    if (!tree -> root) {
        return 0;
    }

    tree -> root = splayNode(tree -> root, key);
    
    return tree -> root -> key == key;
}

//Inserts a new key and makes it the root using splay operations.
int splayInsert(SplayTree *tree, int key) {

    if (!tree -> root) {
        tree -> root = splayNewNode(key);
        tree -> size++;

        return 1;
    }

    tree -> root = splayNode(tree -> root, key);

    if (tree -> root -> key == key) {
        return 0;
    }

    SplayNode *n = splayNewNode(key);

    if (key < tree -> root -> key) {
        n -> right = tree->root;
        n -> left = tree -> root -> left;
        tree -> root -> left = NULL;
    } else {
        n -> left = tree -> root;
        n -> right = tree -> root -> right;
        tree -> root -> right = NULL;
    }

    tree -> root = n;
    tree -> size++;

    return 1;
}

//Removes a key by splaying it to the root and restructuring the tree
int splayDelete(SplayTree *tree, int key) {

    if (!tree -> root) {
        return 0;
    }

    tree -> root = splayNode(tree -> root, key);

    if (tree -> root -> key != key) {
        return 0;
    }

    SplayNode *temp;

    if (!tree -> root -> left) {
        temp = tree -> root;
        tree -> root = tree -> root -> right;
    } else {
        temp = tree -> root;
        
        SplayNode *left = tree -> root -> left;

        left = splayNode(left, key);
        left -> right = tree -> root -> right;
        tree -> root = left;
    }

    free(temp);

    tree -> size--;

    return 1;
}

/* Splay strom je samonastavovací binárny vyhľadávací strom. 
Po každej operácii sa prvok presunie do koreňa pomocou rotácií,
čo zrýchľuje prístup k často používaným prvkom.*/