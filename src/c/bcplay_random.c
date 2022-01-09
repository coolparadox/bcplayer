#include <assert.h>
#include <stdlib.h>
#include <time.h>

#include "bcplay_random.h"

unsigned int bc_random_sample_uniform(unsigned int min, unsigned int max) {
    assert(max >= min);
    srand(time(0));
    unsigned long long int t = rand();
    t *= max - min;
    t += (unsigned long long int) RAND_MAX * min;
    return t / RAND_MAX;
}

