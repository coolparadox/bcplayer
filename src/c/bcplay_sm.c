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

int bc_sm_assess(const struct bc_perception* sight, struct bc_sm_recommendation* result) {

    result->hint.hint = BC_HINT_NOTHING;
    result->sleep = bc_random_sample_uniform(5*60, 15*60);
    switch (_bc_sm_state) {

        case BC_STATE_END:
            log_warning("got an assessment request for the ending state");
            return 0;

        case BC_STATE_START:

            // if (sight->glimpse != BC_GLIMPSE_KIOSK) { log_debug("sm: new state: start"); return 0; }
            log_debug("state: start");
            return 0;

        default:
            panic("unknown state %u", _bc_sm_state);

    }

}
