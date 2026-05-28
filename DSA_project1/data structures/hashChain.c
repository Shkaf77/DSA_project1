#include <stdio.h>
#include <stdlib.h>


//Represents a single element in a linked list used to handle collisions.
typedef struct ChainNode {
    int key;
    struct ChainNode *next;
} ChainNode;


//Represents the hash table with an array of buckets, capacity, and current size.
typedef struct HashChain {
    ChainNode **buckets;
    size_t capacity;
    size_t size;
} HashChain;

//Сomputes the index for a key using modulo operation
static size_t hashChainHash(int key, size_t capacity) {
    return (unsigned int)key % capacity;
}

//Creates a hash table with a specified capacity and initializes buckets
static HashChain *hashChainCreateWithCapacity(size_t capacity) {
    HashChain *h = malloc(sizeof(HashChain));
    if (!h) {
        return NULL;
    }

    h -> buckets = calloc(capacity, sizeof(ChainNode *));
    if (!h -> buckets) {
        free(h);

        return NULL;
    }

    h -> capacity = capacity;
    h -> size = 0;

    return h;
}

//Creates a hash table with a default initial capacity
HashChain *hashChainCreate() {
    return hashChainCreateWithCapacity(16);
}

//Frees all nodes in a linked list.
static void hashChainFreeList(ChainNode *node) {
    while (node) {
        ChainNode *next = node->next;
        free(node);
        node = next;
    }
}

//Frees the entire hash table and all its elements
void hashChainDestroy(HashChain *h) {
    if (!h) {
        return;
    }

    for (size_t i = 0; i < h -> capacity; i++) {
        hashChainFreeList(h -> buckets[i]);
    }

    free(h -> buckets);
    free(h);
}

//Rebuilds the table with a new capacity and redistributes all elements
static int hashChainRehash(HashChain *h, size_t newCapacity) {
    ChainNode **newBuckets = calloc(newCapacity, sizeof(ChainNode *));
    if (!newBuckets) {
        return 0;
    }

    for (size_t i = 0; i < h -> capacity; i++) {
        ChainNode *node = h -> buckets[i];

        while (node) {
            ChainNode *next = node -> next;

            size_t newIndex = hashChainHash(node -> key, newCapacity);
            node -> next = newBuckets[newIndex];
            newBuckets[newIndex] = node;

            node = next;
        }
    }

    free(h -> buckets);

    h -> buckets = newBuckets;
    h -> capacity = newCapacity;

    return 1;
}

//Searches for a key in the appropriate bucket using linked list traversal
int hashChainSearch(HashChain *h, int key) {
    if (!h) {
        return 0;
    }

    size_t index = hashChainHash(key, h -> capacity);
    ChainNode *node = h -> buckets[index];

    while (node) {
        if (node -> key == key) {
            return 1;
        }

        node = node -> next;
    }

    return 0;
}

//Inserts a new key into the table and resizes it if load factor exceeds 0.75.
int hashChainInsert(HashChain *h, int key) {
    if (!h) {
        return 0;
    }

    if (hashChainSearch(h, key)) {
        return 0;
    }

    double loadFactor = (double)(h -> size + 1) / (double)h -> capacity;

    if (loadFactor > 0.75) {
        if (!hashChainRehash(h, h -> capacity * 2)) {
            return 0;
        }
    }

    size_t index = hashChainHash(key, h -> capacity);

    ChainNode *node = malloc(sizeof(ChainNode));

    if (!node) {
        return 0;
    }

    node -> key = key;
    node -> next = h -> buckets[index];
    h -> buckets[index] = node;

    h -> size++;

    return 1;
}

//Removes a key from the table and shrinks it if load factor drops below 0.20.
int hashChainDelete(HashChain *h, int key) {
    if (!h) {
        return 0;
    }

    size_t index = hashChainHash(key, h -> capacity);
    ChainNode *node = h -> buckets[index];
    ChainNode *prev = NULL;

    while (node) {
        if (node -> key == key) {
            if (prev) {
                prev -> next = node -> next;
            } else {
                h -> buckets[index] = node -> next;
            }

            free(node);

            h -> size--;

            double loadFactor = (double)h -> size / (double)h -> capacity;

            if (h -> capacity > 16 && loadFactor < 0.20) {
                hashChainRehash(h, h -> capacity / 2);
            }

            return 1;
        }

        prev = node;
        node = node -> next;
    }

    return 0;
}

//Returns the number of elements stored in the hash table.
size_t hashChainSize(HashChain *h) {
    return h ? h -> size : 0;
}

/* Ide o hash tabuľku s riešením kolízií pomocou reťazcov. 
Každý prvok sa ukladá do zoznamu v príslušnom bucket-e. 
Pri kolízii sa prvky pridávajú do zoznamu. 
Pri zmene load factor sa tabuľka dynamicky mení.*/