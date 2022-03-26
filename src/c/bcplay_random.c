/*
 * Copyright 2022 Rafael Lorandi <coolparadox@gmail.com>
 *
 * This file is part of bcplay.
 *
 * bcplay is free software: you can redistribute it and/or modify it under the terms of the GNU
 * General Public License as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * bcplay is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with bcplay. If not, see
 * <https://www.gnu.org/licenses/>.
 */

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

