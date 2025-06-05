#include <rand.h>

#define A 1664525
#define C 1013904223
#define M 4294967296 // 2^32

static unsigned int seed = 1;

void srand(unsigned int new_seed) {
    seed = new_seed;
}

// Returns a number between 0 and max-1
int rand_between(int max) {
    seed = (A * seed + C) % M;
    return seed % max;
}