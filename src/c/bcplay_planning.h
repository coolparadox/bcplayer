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

#ifndef BCPLAY_PLANNING_H_SEEN
#define BCPLAY_PLANNING_H_SEEN

#include "bcplay_perception_types.h"
#include "bcplay_planning_types.h"

void bc_planning_init();

enum bc_planning_states bc_planning_get_state();

int bc_planning_assess(const struct bc_perception* sight, struct bc_planning_recommendation* advice);

#endif  // BCPLAY_PLANNING_H_SEEN
