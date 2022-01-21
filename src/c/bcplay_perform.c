#include <unistd.h>

#include <xdo.h>

#include "bcplay_log.h"

#include "bcplay_perform.h"

#define BC_MODULE "perform"

int _bc_perform_keyboard_click(const xdo_t* xdo, const struct bc_hint_keyboark_click* detail) {
    if (xdo_send_keysequence_window(xdo, CURRENTWINDOW, detail->key, 100000)) fail("cannot send key sequence");;
    return 0;

}

int bc_perform(const struct bc_planning_hint* hints) {
    xdo_t* xdo = xdo_new(NULL); if (!xdo) fail("xdo: cannot instantiate");
#define cleanup_fail(...) { xdo_free(xdo); fail(__VA_ARGS__); }
     do {
        switch (hints->type) {
            case 0: break;
            case BC_HINT_KEYBOARD_CLICK:
                if(_bc_perform_keyboard_click(xdo, &hints->detail.keyboard_click)) cleanup_fail("cannot perform keyboard click");
                break;
            default: panic("unknown hint type %i", hints->type);
        }
        log_warning("TODO: implement human pauses between events");
        usleep(10000);
    } while (hints++->type);
    xdo_free(xdo);
}

