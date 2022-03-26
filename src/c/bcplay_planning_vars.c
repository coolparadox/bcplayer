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

#include <time.h>

#include "bcplay_planning_types.h"

enum bc_planning_states _bc_planning_state;
int _bc_planning_loading_wait;
int _bc_planning_loading_wait_prev;
int _bc_planning_error_wait;
int _bc_planning_error_wait_prev;
int _bc_planning_unknown_wait;
int _bc_planning_unknown_wait_prev;
int _bc_planning_full_energy_wait;
time_t _bc_planning_next_character_selection;
int _bc_planning_characters_scroll_count;
int _bc_planning_game_selected;

