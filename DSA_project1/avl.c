#include <stdio.h>
#include <stdlib.h>

//Stores a single tree node: key, height, and pointers to left and right children
typedef struct AVLNode {
    int key;
    int height;
    struct AVLNode *left;
    struct AVLNode *right;
} AVLNode;

//Represents the whole tree: contains the root and the number of elements
typedef struct AVLTree {
    AVLNode *root;
    size_t size;
} AVLTree;

//Returns the height of a node or 0 if the node is NULL
static int avlHeightNode(AVLNode *n) {
    return n ? n -> height : 0;
}

//Returns the maximum of two integers
static int avlMax(int a, int b) {
    return (a > b) ? a : b;
}

//Creates and initializes a new node with a given key and height = 1
static AVLNode *avlNewNode(int key) {
    AVLNode *n = (AVLNode *)malloc(sizeof(AVLNode));
    
    if (!n) {
        return NULL;
    }
    
    n -> key = key;
    n -> height = 1;
    n -> left = NULL;
    n -> right = NULL;
    return n;
}

//Recalculates the height of a node based on its children
static void avlUpdateHeight(AVLNode *n) {
    if (n) {
        n -> height = 1 + avlMax(avlHeightNode(n -> left), avlHeightNode(n -> right));
    }
}

//Calculate the balance factor (left subtree height minus right subtree height)
static int avlBalanceFactor(AVLNode *n) {
    return n ? avlHeightNode(n -> left) - avlHeightNode(n -> right) : 0;
}

//Performs a right rotation to fix left-heavy imbalance
static AVLNode *avlRotateRight(AVLNode *y) {
    AVLNode *x = y -> left;
    AVLNode *t2 = x -> right;

    x -> right = y;
    y -> left = t2;

    avlUpdateHeight(y);
    avlUpdateHeight(x);

    return x;
}

//Performs a left rotation to fix right-heavy imbalance
static AVLNode *avlRotateLeft(AVLNode *x) {
    AVLNode *y = x -> right;
    AVLNode *t2 = y -> left;

    y -> left = x;
    x -> right = t2;

    avlUpdateHeight(x);
    avlUpdateHeight(y);

    return y;
}

//Checks the balance factor and applies necessary rotations to restore AVL property
static AVLNode *avlBalance(AVLNode *node) {
    avlUpdateHeight(node);

    int bf = avlBalanceFactor(node);

    //Left-heavy case
    if (bf > 1) {
        if (avlBalanceFactor(node -> left) < 0) {
            node -> left = avlRotateLeft(node -> left);
        }

        return avlRotateRight(node);
    }

    //Right-heavy case
    if (bf < -1) {
        if (avlBalanceFactor(node -> right) > 0) {
            node -> right = avlRotateRight(node -> right);
        }

        return avlRotateLeft(node);
    }

    return node;
}

//Recursively inserts a key like in BST and then balances the tree
static AVLNode *avlInsertNode(AVLNode *node, int key, int *inserted) {
    if (!node) {
        *inserted = 1;
        return avlNewNode(key);
    }

    if (key < node -> key) {
        node -> left = avlInsertNode(node -> left, key, inserted);
    } else if (key > node -> key) {
        node -> right = avlInsertNode(node -> right, key, inserted);
    } else {
        return node;
    }

    return avlBalance(node);
}

//Finds the node with the minimum key (leftmost node)
static AVLNode *avlMinNode(AVLNode *node) {
    while (node && node -> left) {
        node = node -> left;
    }

    return node;
}

//Removes a node by key (handling all cases) and then rebalances the tree
static AVLNode *avlDeleteNode(AVLNode *node, int key, int *deleted) {
    if (!node) return NULL;

    if (key < node->key) {
        node -> left = avlDeleteNode(node -> left, key, deleted);
    } else if (key > node -> key) {
        node -> right = avlDeleteNode(node -> right, key, deleted);
    } else {
        *deleted = 1;

        if (!node -> left || !node -> right) {
            AVLNode *temp = node -> left ? node -> left : node -> right;

            free(node);

            return temp;
        }

        AVLNode *temp = avlMinNode(node->right);
        node -> key = temp -> key;

        int dummy = 0;

        node -> right = avlDeleteNode(node -> right, temp -> key, &dummy);
    }

    return avlBalance(node);
}

//Performs standard binary search tree lookup
static int avlSearchNode(AVLNode *node, int key) {
    while (node) {
        if (key < node -> key) {
            node = node -> left;
        } else if (key > node -> key) {
            node = node -> right;
        } else {
            return 1;
        }
    }

    return 0;
}

//Recursively frees all nodes in the tree
static void avlFreeNode(AVLNode *node) {
    if (!node) {
        return;
    }

    avlFreeNode(node -> left);
    avlFreeNode(node -> right);

    free(node);
}

//Creates and initializes an empty AVL tree.
AVLTree *avlCreate(void) {
    AVLTree *t = malloc(sizeof(AVLTree));

    t -> root = NULL;
    t -> size = 0;
    
    return t;
}

//Frees the entire tree and all its nodes
void avlDestroy(AVLTree *tree) {
    avlFreeNode(tree -> root);

    free(tree);
}

//Inserts a key and increases size if insertion was successful
int avlInsert(AVLTree *tree, int key) {
    int inserted = 0;

    tree -> root = avlInsertNode(tree -> root, key, &inserted);

    if (inserted) {
        tree -> size++;
    }

    return inserted;
}

//Deletes a key and decreases size if deletion was successful
int avlDelete(AVLTree *tree, int key) {
    int deleted = 0;

    tree -> root = avlDeleteNode(tree -> root, key, &deleted);

    if (deleted) {
        tree -> size--;
    }

    return deleted;
}

//Checks whether a key exists in the tree.
int avlSearch(AVLTree *tree, int key) {
    return avlSearchNode(tree -> root, key);
}

//Returns the number of elements in the tree
size_t avlSize(AVLTree *tree) {
    return tree -> size;
}

/* Ide o samovyvažovací binárny vyhľadávací strom. 
Po každej operácii sa počíta balance factor a v prípade potreby sa vykonajú rotácie, 
aby sa zachovala logaritmická výška stromu. */