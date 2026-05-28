#include <stdio.h>
#include <stdlib.h>


//Defines the state of each cell: empty, occupied, or deleted
typedef enum {
    hashDoubleEmpty = 0,
    hashDoubleOccupied = 1,
    hashDoubleDeleted = 2
} HashDoubleState;

//Represents the hash table with an array of values, state array, capacity, and size
typedef struct HashDouble {
    int *table;
    HashDoubleState *state;
    size_t capacity;
    size_t size;
} HashDouble;

//Checks whether a number is prime
static int hashDoubleIsPrime(size_t x) {
    if (x < 2) return 0;

    for (size_t i = 2; i * i <= x; i++) {
        if (x % i == 0) {
            return 0;
        }
    }

    return 1;
}

//Finds the next prime number greater than or equal to a given value
static size_t hashDoubleNextPrime(size_t x) {
    while (!hashDoubleIsPrime(x)) {
        x++;
    }
    return x;
}

//Computes the primary hash index using modulo operation
static size_t hashDoubleHash1(int key, size_t capacity) {
    return (unsigned int)key % capacity;
}

//Computes the secondary hash step size for probing
static size_t hashDoubleHash2(int key, size_t capacity) {
    return 1 + ((unsigned int)key % (capacity - 1));
}

//Creates a hash table with a specified capacity, ensuring it's prime, and initializes all states as empty
static HashDouble *hashDoubleCreateWithCapacity(size_t capacity) {
    HashDouble *h = malloc(sizeof(HashDouble));

    if (!h) {
        return NULL;
    }

    capacity = hashDoubleNextPrime(capacity);

    h -> table = malloc(sizeof(int) * capacity);
    h -> state = malloc(sizeof(HashDoubleState) * capacity);

    if (!h -> table || !h -> state) {
        free(h -> table);
        free(h -> state);
        free(h);
        return NULL;
    }

    for (size_t i = 0; i < capacity; i++) {
        h -> state[i] = hashDoubleEmpty;
        h -> table[i] = 0;
    }

    h -> capacity = capacity;
    h -> size = 0;

    return h;
}

//Creates a hash table with a default initial capacity
HashDouble *hashDoubleCreate() {
    return hashDoubleCreateWithCapacity(17);
}

//Frees all allocated memory for the hash table
void hashDoubleDestroy(HashDouble *h) {
    if (!h) {
        return;
    }

    free(h -> table);
    free(h -> state);
    free(h);
}

//Frees all allocated memory for the hash table
static int hashDoubleInsertInternal(HashDouble *h, int key) {
    size_t h1 = hashDoubleHash1(key, h -> capacity);
    size_t h2 = hashDoubleHash2(key, h -> capacity);
    size_t firstDeleted = (size_t)-1;

    for (size_t i = 0; i < h -> capacity; i++) {
        size_t index = (h1 + i * h2) % h -> capacity;

        if (h -> state[index] == hashDoubleDeleted && firstDeleted == (size_t)-1) {
            firstDeleted = index;
        }

        //If we find an empty cell, we can insert the key there. If we previously found a deleted cell, we will insert there instead.
        if (h -> state[index] == hashDoubleEmpty) {
            if (firstDeleted != (size_t) - 1) {
                index = firstDeleted;
            }

            h -> table[index] = key;
            h -> state[index] = hashDoubleOccupied;
            h -> size++;

            return 1;
        }
    }

    if (firstDeleted != (size_t)-1) {
        h -> table[firstDeleted] = key;
        h -> state[firstDeleted] = hashDoubleOccupied;
        h -> size++;
        return 1;
    }

    return 0;
}

//Rebuilds the table with a new capacity and reinserts all elements.
static int hashDoubleRehash(HashDouble *h, size_t newCapacity) {
    HashDouble *newHash = hashDoubleCreateWithCapacity(newCapacity);
     
    if (!newHash) {
        return 0;
    }

    for (size_t i = 0; i < h -> capacity; i++) {
        if (h -> state[i] == hashDoubleOccupied) {
            hashDoubleInsertInternal(newHash, h -> table[i]);
        }
    }

    free(h -> table);
    free(h -> state); 

    h -> table = newHash -> table;
    h -> state = newHash -> state;
    h -> capacity = newHash -> capacity;
    h -> size = newHash -> size;

    free(newHash);

    return 1;
}

//Rebuilds the table with a new capacity and reinserts all elements
int hashDoubleSearch(HashDouble *h, int key) {
    if (!h) {
        return 0;
    }

    size_t h1 = hashDoubleHash1(key, h -> capacity);
    size_t h2 = hashDoubleHash2(key, h -> capacity);

    for (size_t i = 0; i < h->capacity; i++) {
        size_t index = (h1 + i * h2) % h->capacity;

        if (h -> state[index] == hashDoubleEmpty) {
            return 0;
        }

        if (h -> state[index] == hashDoubleOccupied && h -> table[index] == key) {
            return 1;
        }
    }

    return 0;
}

//Searches for a key using double hashing probing
int hashDoubleInsert(HashDouble *h, int key) {
    if (!h) {
        return 0;
    }

    if (hashDoubleSearch(h, key)) {
        return 0;
    }

    double loadFactor = (double)(h -> size + 1) / (double)h -> capacity;
    if (loadFactor > 0.60) {
        if (!hashDoubleRehash(h, h -> capacity * 2)) {
            return 0;
        } 
    }

    return hashDoubleInsertInternal(h, key);
}

//Marks a key as deleted and shrinks the table if load factor drops below 0.20
int hashDoubleDelete(HashDouble *h, int key) {
    if (!h) {
        return 0;
    }

    size_t h1 = hashDoubleHash1(key, h -> capacity);
    size_t h2 = hashDoubleHash2(key, h -> capacity);

    for (size_t i = 0; i < h -> capacity; i++) {
        size_t index = (h1 + i * h2) % h -> capacity;

        if (h -> state[index] == hashDoubleEmpty) {
            return 0;
        }

        if (h -> state[index] == hashDoubleOccupied && h->table[index] == key) {
            h -> state[index] = hashDoubleDeleted;
            h -> size--;

            double loadFactor = (double)h -> size / (double)h -> capacity;

            if (h -> capacity > 17 && loadFactor < 0.20) {
                hashDoubleRehash(h, h->capacity / 2);
            }

            return 1;
        }
    }

    return 0;
}

//Returns the number of elements stored in the hash table
size_t hashDoubleSize(HashDouble *h) {
    return h ? h -> size : 0;
}

/* Ide o hash tabuľku s otvorenou adresáciou a dvojitým hashovaním. 
Pri kolízii sa používa druhá hash funkcia na výpočet ďalšej pozície. 
Každá bunka má stav: prázdna, obsadená alebo zmazaná. 
Pri vysokom load factore sa tabuľka zväčšuje, pri nízkom zmenšuje. */