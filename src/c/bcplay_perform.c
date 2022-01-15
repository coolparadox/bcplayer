#include "bcplay_perform.h"

#include "bcplay_log.h"

#define BC_MODULE "perform"

int bc_perform(const struct bc_sm_hint* hint) {
    switch (hint->hint) {
        case BC_HINT_NOTHING:
            log_debug("nothing");
            return 0;
        default:
            fail("unknown hint %i", hint->hint);
    }
}

