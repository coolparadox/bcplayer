#include <unistd.h>

#include "bcplay_log.h"

#include "bcplay_perform.h"

#define BC_MODULE "perform"

int bc_perform(const struct bc_planning_hint* hints) {
    do {
        switch (hints->type) {
            case BC_HINT_END:
                break;
            case BC_HINT_KEYBOARD_CLICK:
                // log_debug("hint: keyboard click");
                log_warning("hint: keyboard click: not implemented");
            default:
                panic("unknown hint type %i", hints->type);
        }
        // FIXME: implement human pauses
        sleep(1);
    } while (hints++->type);
}

