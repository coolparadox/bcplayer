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
