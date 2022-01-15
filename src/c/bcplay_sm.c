#include "bcplay_log.h"
#include "bcplay_random.h"

#include "bcplay_sm.h"

#define BC_MODULE "sm"

extern enum bc_sm_states _bc_sm_state;

void bc_sm_init() {
    _bc_sm_state = BC_STATE_START;
}

enum bc_sm_states bc_sm_get_state() {
    return _bc_sm_state;
}

int bc_sm_assess_black(const union bc_perception_detail* detail, struct bc_sm_recommendation* advice);

int bc_sm_assess(const struct bc_perception* sight, struct bc_sm_recommendation* advice) {
    advice->hint.hint = BC_HINT_NOTHING;
    advice->sleep = bc_random_sample_uniform(5*60, 15*60);
    switch (sight->glimpse) {
        case BC_GLIMPSE_UNKNOWN: return 0;
        case BC_GLIMPSE_BLACK: return bc_sm_assess_black(&sight->detail, advice);
    }
    panic("unknown glimpse: %d", sight->glimpse);
}

int bc_sm_assess_black(const union bc_perception_detail* detail, struct bc_sm_recommendation* advice) {
    // The vnc has just started.
    log_debug("advice: just wait (for the kiosk window)");
    advice->sleep = 2;
    return 0;
}
