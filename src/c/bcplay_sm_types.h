#ifndef BCPLAY_MACHINE_TYPES_H_SEEN
#define BCPLAY_MACHINE_TYPES_H_SEEN

enum bc_sm_states {
    BC_STATE_START,
    BC_STATE_END,  // Game over.
};

enum bc_sm_hints {
    BC_HINT_UNKNOWN,
    BC_HINT_NOTHING,  // Just wait
};

struct bc_sm_recommendation {
    enum bc_sm_hints hint;
    unsigned int sleep;
    union {} detail;
};

#endif  // BCPLAY_MACHINE_TYPES_H_SEEN
