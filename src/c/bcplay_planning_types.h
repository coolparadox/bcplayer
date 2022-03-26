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

#ifndef BCPLAY_PLANNING_TYPES_H_SEEN
#define BCPLAY_PLANNING_TYPES_H_SEEN

#include "bcplay_conf.h"
#include "bcplay_hint_types.h"

enum bc_planning_states {
    BC_STATE_START = 0,
    BC_STATE_END = 1,  // Game over.
};

struct bc_planning_recommendation {
    struct bc_planning_hint hints[BC_PLANNING_HINTS_SIZE];
    unsigned int sleep;
};

#endif  // BCPLAY_PLANNING_TYPES_H_SEEN
