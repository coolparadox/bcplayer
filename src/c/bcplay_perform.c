#include "bcplay_perform.h"

#include "bcplay_log.h"

int bc_perform(const struct bc_sm_hint* hint) {
    switch (hint->hint) {
        case BC_HINT_NOTHING:
            break;
        default:
            FAIL("unknown hint %i", hint->hint);
    }
}

