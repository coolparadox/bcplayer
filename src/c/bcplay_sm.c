#include "bcplay_log.h"
#include "bcplay_random.h"

#include "bcplay_sm.h"

extern enum bc_sm_states bc_sm_state;

void bc_sm_init() {
    bc_sm_state = BC_STATE_START;
}

enum bc_sm_states bc_sm_get_state() {
    return bc_sm_state;
}

int bc_sm_assess(const struct bc_perception* sight, struct bc_sm_recommendation* result) {

    result->hint.hint = BC_HINT_NOTHING;
    result->sleep = bc_random_sample_uniform(5, 15);
    switch (bc_sm_state) {

        case BC_STATE_END:
            log_warning("I should not assess the ending state");
            result->hint.hint = BC_HINT_NOTHING;
            result->sleep = bc_random_sample_uniform(5, 15);
            return 0;

        case BC_STATE_START:

            if (sight->glimpse != BC_GLIMPSE_KIOSK) {
                // Wait for the kiosk window to appear.
                result->hint.hint = BC_HINT_NOTHING;
                result->sleep = bc_random_sample_uniform(5, 15);
                return 0;
            }
            // The kiosk window has just appeared.
            // TODO: now what?
            return 0;

        default:
            PANIC("unknown state %u", bc_sm_state);

    }

}
