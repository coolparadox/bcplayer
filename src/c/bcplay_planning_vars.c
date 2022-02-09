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

