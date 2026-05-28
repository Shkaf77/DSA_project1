#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct AVLTree AVLTree;
AVLTree *avlCreate(void);
void avlDestroy(AVLTree *tree);
int avlInsert(AVLTree *tree, int key);
int avlDelete(AVLTree *tree, int key);
int avlSearch(AVLTree *tree, int key);

typedef struct SplayTree SplayTree;
SplayTree *splayCreate(void);
void splayDestroy(SplayTree *tree);
int splayInsert(SplayTree *tree, int key);
int splayDelete(SplayTree *tree, int key);
int splaySearch(SplayTree *tree, int key);

typedef struct HashChain HashChain;
HashChain *hashChainCreate(void);
void hashChainDestroy(HashChain *h);
int hashChainInsert(HashChain *h, int key);
int hashChainDelete(HashChain *h, int key);
int hashChainSearch(HashChain *h, int key);

typedef struct HashDouble HashDouble;
HashDouble *hashDoubleCreate(void);
void hashDoubleDestroy(HashDouble *h);
int hashDoubleInsert(HashDouble *h, int key);
int hashDoubleDelete(HashDouble *h, int key);
int hashDoubleSearch(HashDouble *h, int key);

static double getMicroseconds(clock_t start, clock_t end, int n) {
    double totalSeconds = (double)(end - start) / CLOCKS_PER_SEC;
    return (totalSeconds / n) * 1000000.0;
}

static void generateSorted(int *data, int n) {
    for (int i = 0; i < n; i++) {
        data[i] = i + 1;
    }
}

static void generateReverse(int *data, int n) {
    for (int i = 0; i < n; i++) {
        data[i] = n - i;
    }
}

static void generateRandom(int *data, int n) {
    for (int i = 0; i < n; i++) {
        data[i] = i + 1;
    }

    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = data[i];

        data[i] = data[j];
        data[j] = temp;
    }
}

static void benchmarkAvl(const char *scenario, int *data, int n, int repeats) {
    double total;
    clock_t start, end;

    total = 0.0;

    for (int r = 0; r < repeats; r++) {
        AVLTree *tree = avlCreate();

        if (!tree) return;

        start = clock();

        for (int i = 0; i < n; i++) {
            avlInsert(tree, data[i]);
        }

        end = clock();

        total += getMicroseconds(start, end, n);

        avlDestroy(tree);
    }

    printf("AVL,%s,%d,INSERT,%.3f\n", scenario, n, total / repeats);

    total = 0.0;

    for (int r = 0; r < repeats; r++) {
        AVLTree *tree = avlCreate();

        if (!tree) return;

        for (int i = 0; i < n; i++) {
            avlInsert(tree, data[i]);
        }

        start = clock();

        for (int i = 0; i < n; i++) {
            avlSearch(tree, data[i]);
        }

        end = clock();

        total += getMicroseconds(start, end, n);

        avlDestroy(tree);
    }

    printf("AVL,%s,%d,SEARCH,%.3f\n", scenario, n, total / repeats);

    total = 0.0;

    for (int r = 0; r < repeats; r++) {
        AVLTree *tree = avlCreate();

        if (!tree) return;

        for (int i = 0; i < n; i++) {
            avlInsert(tree, data[i]);
        }

        start = clock();

        for (int i = 0; i < n; i++) {
            avlDelete(tree, data[i]);
        }

        end = clock();

        total += getMicroseconds(start, end, n);

        avlDestroy(tree);
    }

    printf("AVL,%s,%d,DELETE,%.3f\n", scenario, n, total / repeats);
}

static void benchmarkSplay(const char *scenario, int *data, int n, int repeats) {
    double total;
    clock_t start, end;

    total = 0.0;
    for (int r = 0; r < repeats; r++) {
        SplayTree *tree = splayCreate();

        if (!tree) return;

        start = clock();

        for (int i = 0; i < n; i++) {
            splayInsert(tree, data[i]);
        }

        end = clock();

        total += getMicroseconds(start, end, n);

        splayDestroy(tree);
    }

    printf("SPLAY,%s,%d,INSERT,%.3f\n", scenario, n, total / repeats);

    total = 0.0;
    for (int r = 0; r < repeats; r++) {
        SplayTree *tree = splayCreate();

        if (!tree) return;

        for (int i = 0; i < n; i++) {
            splayInsert(tree, data[i]);
        }

        start = clock();

        for (int i = 0; i < n; i++) {
            splaySearch(tree, data[i]);
        }

        end = clock();

        total += getMicroseconds(start, end, n);

        splayDestroy(tree);
    }

    printf("SPLAY,%s,%d,SEARCH,%.3f\n", scenario, n, total / repeats);

    total = 0.0;
    for (int r = 0; r < repeats; r++) {
        SplayTree *tree = splayCreate();

        if (!tree) return;

        for (int i = 0; i < n; i++) {
            splayInsert(tree, data[i]);
        }

        start = clock();

        for (int i = 0; i < n; i++) {
            splayDelete(tree, data[i]);
        }

        end = clock();

        total += getMicroseconds(start, end, n);

        splayDestroy(tree);
    }

    printf("SPLAY,%s,%d,DELETE,%.3f\n", scenario, n, total / repeats);
}

static void benchmarkHashChain(const char *scenario, int *data, int n, int repeats) {
    double total;
    clock_t start, end;

    total = 0.0;

    for (int r = 0; r < repeats; r++) {
        HashChain *h = hashChainCreate();

        if (!h) return;

        start = clock();

        for (int i = 0; i < n; i++) {
            hashChainInsert(h, data[i]);
        }

        end = clock();

        total += getMicroseconds(start, end, n);

        hashChainDestroy(h);
    }

    printf("HASH CHAIN,%s,%d,INSERT,%.3f\n", scenario, n, total / repeats);

    total = 0.0;

    for (int r = 0; r < repeats; r++) {
        HashChain *h = hashChainCreate();

        if (!h) return;

        for (int i = 0; i < n; i++) {
            hashChainInsert(h, data[i]);
        }

        start = clock();

        for (int i = 0; i < n; i++) {
            hashChainSearch(h, data[i]);
        }

        end = clock();

        total += getMicroseconds(start, end, n);

        hashChainDestroy(h);
    }

    printf("HASH CHAIN,%s,%d,SEARCH,%.3f\n", scenario, n, total / repeats);

    total = 0.0;

    for (int r = 0; r < repeats; r++) {
        HashChain *h = hashChainCreate();

        if (!h) return;

        for (int i = 0; i < n; i++) {
            hashChainInsert(h, data[i]);
        }

        start = clock();

        for (int i = 0; i < n; i++) {
            hashChainDelete(h, data[i]);
        }

        end = clock();

        total += getMicroseconds(start, end, n);

        hashChainDestroy(h);
    }

    printf("HASH CHAIN,%s,%d,DELETE,%.3f\n", scenario, n, total / repeats);
}

static void benchmarkHashDouble(const char *scenario, int *data, int n, int repeats) {
    double total;
    clock_t start, end;

    total = 0.0;
    for (int r = 0; r < repeats; r++) {
        HashDouble *h = hashDoubleCreate();

        if (!h) return;

        start = clock();

        for (int i = 0; i < n; i++) {
            hashDoubleInsert(h, data[i]);
        }

        end = clock();

        total += getMicroseconds(start, end, n);

        hashDoubleDestroy(h);
    }

    printf("HASH DOUBLE,%s,%d,INSERT,%.3f\n", scenario, n, total / repeats);

    total = 0.0;

    for (int r = 0; r < repeats; r++) {
        HashDouble *h = hashDoubleCreate();

        if (!h) return;

        for (int i = 0; i < n; i++) {
            hashDoubleInsert(h, data[i]);
        }

        start = clock();

        for (int i = 0; i < n; i++) {
            hashDoubleSearch(h, data[i]);
        }

        end = clock();

        total += getMicroseconds(start, end, n);
        hashDoubleDestroy(h);
    }

    printf("HASH DOUBLE,%s,%d,SEARCH,%.3f\n", scenario, n, total / repeats);

    total = 0.0;

    for (int r = 0; r < repeats; r++) {
        HashDouble *h = hashDoubleCreate();

        if (!h) {
            return;
        }

        for (int i = 0; i < n; i++) {
            hashDoubleInsert(h, data[i]);
        }

        start = clock();

        for (int i = 0; i < n; i++) {
            hashDoubleDelete(h, data[i]);
        }

        end = clock();

        total += getMicroseconds(start, end, n);

        hashDoubleDestroy(h);
    }

    printf("HASH DOUBLE,%s,%d,DELETE,%.3f\n", scenario, n, total / repeats);
}

static void runScenario(const char *scenario, int *data, int n, int repeats) {
    benchmarkAvl(scenario, data, n, repeats);
    benchmarkSplay(scenario, data, n, repeats);
    benchmarkHashChain(scenario, data, n, repeats);
    benchmarkHashDouble(scenario, data, n, repeats);
}

int main(void) {
    int sizes[] = {1000, 10000, 20000, 40000, 60000, 80000, 100000};
    int count = sizeof(sizes) / sizeof(sizes[0]);
    int repeats = 10;

    srand((unsigned int)time(NULL));

    for (int s = 0; s < count; s++) {
        int n = sizes[s];
        int *data = malloc(sizeof(int) * n);

        if (!data) {
            return 1;
        }

        generateSorted(data, n);
        runScenario("SORTED", data, n, repeats);

        generateRandom(data, n);
        runScenario("RANDOM", data, n, repeats);

        generateReverse(data, n);
        runScenario("REVERSE", data, n, repeats);

        free(data);
    }

    return 0;
}

/* Tento súbor realizuje benchmark. 
Generuje rôzne typy vstupných dát, 
spúšťa operácie insert, 
search a delete pre všetky implementované štruktúry, 
opakuje každý test 10-krát, 
spriemeruje výsledok a vypisuje čas jednej operácie v mikrosekundách.*/