#include <unistd.h>

#include "bcplay_log.h"

#include "bcplay_perform.h"

#define BC_MODULE "perform"

int bc_perform(const struct bc_planning_hint* hints) {
    do {
        switch (hints->type) {
            case 0: break;
            case BC_HINT_KEYBOARD_CLICK: log_warning("hint: keyboard click: not implemented"); break;
            default: panic("unknown hint type %i", hints->type);
        }
        sleep(1); // FIXME: implement human pauses
    } while (hints++->type);
}

