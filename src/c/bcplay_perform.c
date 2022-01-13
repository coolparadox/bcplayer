#include "bcplay_perform.h"

#include "bcplay_log.h"

int bc_perform(const struct bc_sm_hint* hint) {
    switch (hint->hint) {
        case BC_HINT_NOTHING:
            return 0;
        default:
            FAIL("unknown hint %i", hint->hint);
    }
}

