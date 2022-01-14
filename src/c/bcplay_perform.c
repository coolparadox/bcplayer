#include "bcplay_perform.h"

#include "bcplay_log.h"

int bc_perform(const struct bc_sm_hint* hint) {
    switch (hint->hint) {
        case BC_HINT_NOTHING:
            log_debug("perform: nothing");
            return 0;
        default:
            fail("unknown hint %i", hint->hint);
    }
}

