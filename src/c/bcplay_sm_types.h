#ifndef BCPLAY_SM_TYPES_H_SEEN
#define BCPLAY_SM_TYPES_H_SEEN

#include "bcplay_hint_types.h"

enum bc_sm_states {
    BC_STATE_START = 0,
    BC_STATE_END = 1,  // Game over.
};

struct bc_sm_recommendation {
    struct bc_sm_hint hint;
    unsigned int sleep;
};

#endif  // BCPLAY_SM_TYPES_H_SEEN
