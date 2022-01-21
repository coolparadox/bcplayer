#include <string.h>

#include "bcplay_conf.h"
#include "bcplay_log.h"
#include "bcplay_random.h"

#include "bcplay_planning.h"

#define BC_MODULE "planning"

extern enum bc_planning_states _bc_planning_state;

void bc_planning_init() {
    _bc_planning_state = BC_STATE_START;
}

enum bc_planning_states bc_planning_get_state() {
    return _bc_planning_state;
}

int _bc_planning_assess_black(const union bc_perception_detail* detail, struct bc_planning_recommendation* advice);
int _bc_planning_assess_kiosk_clean(const union bc_perception_detail* detail, struct bc_planning_recommendation* advice);

int bc_planning_assess(const struct bc_perception* sight, struct bc_planning_recommendation* advice) {
    memset(advice->hints, 0, BC_PLANNING_HINTS_SIZE);
    advice->sleep = bc_random_sample_uniform(5*60, 15*60);
    switch (sight->glimpse) {
        case BC_GLIMPSE_UNKNOWN: return 0;
        case BC_GLIMPSE_BLACK: return _bc_planning_assess_black(&sight->detail, advice);
        case BC_GLIMPSE_KIOSK_UPDATED: log_warning("'kiosk updated' assessment not implemented"); return 0;
        case BC_GLIMPSE_KIOSK_CLEAN: return _bc_planning_assess_kiosk_clean(&sight->detail, advice);
    }
    panic("unknown glimpse: %d", sight->glimpse);
}

int _bc_planning_assess_black(const union bc_perception_detail* detail, struct bc_planning_recommendation* advice) {
    // The vnc has just started.
    log_debug("advice: just wait (for the kiosk window)");
    advice->sleep = 2;
    return 0;
}

int _bc_planning_assess_kiosk_clean(const union bc_perception_detail* detail, struct bc_planning_recommendation* advice) {
    // The kiosk browser has just appeared.
    log_debug("advice: type the game url");
    struct bc_planning_hint* hint = advice->hints;

    // Bring Firefox URL pane
    hint->type = BC_HINT_KEYBOARD_CLICK;
    hint->detail.keyboard_click.symbol = XK_l;
    hint->detail.keyboard_click.modifier = XK_Control_L;
    hint++;

    log_warning("'kiosk clean' assessment not implemented");
    advice->sleep = 60;
    return 0;
}

/*

KeyPress event, serial 33, synthetic NO, window 0xe00001,
    root 0x38c, subw 0x0, time 24980250, (174,176), root:(414,439),
    state 0x10, keycode 37 (keysym 0xffe3, Control_L), same_screen YES,
    XLookupString gives 0 bytes:
    XmbLookupString gives 0 bytes:
    XFilterEvent returns: False

KeyPress event, serial 33, synthetic NO, window 0xe00001,
    root 0x38c, subw 0x0, time 24980406, (174,176), root:(414,439),
    state 0x14, keycode 46 (keysym 0x6c, l), same_screen YES,
    XLookupString gives 1 bytes: (0c) "
                                       "
    XmbLookupString gives 1 bytes: (0c) "
                                         "
    XFilterEvent returns: False

KeyRelease event, serial 33, synthetic NO, window 0xe00001,
    root 0x38c, subw 0x0, time 24980453, (174,176), root:(414,439),
    state 0x14, keycode 46 (keysym 0x6c, l), same_screen YES,
    XLookupString gives 1 bytes: (0c) "
                                       "
    XFilterEvent returns: False

KeyRelease event, serial 33, synthetic NO, window 0xe00001,
    root 0x38c, subw 0x0, time 24980625, (174,176), root:(414,439),
    state 0x14, keycode 37 (keysym 0xffe3, Control_L), same_screen YES,
    XLookupString gives 0 bytes:
    XFilterEvent returns: False



KeyPress event, serial 33, synthetic NO, window 0xe00001,
    root 0x38c, subw 0x0, time 25432671, (173,175), root:(439,464),
    state 0x10, keycode 46 (keysym 0x6c, l), same_screen YES,
    XLookupString gives 1 bytes: (6c) "l"
    XmbLookupString gives 1 bytes: (6c) "l"
    XFilterEvent returns: False

KeyRelease event, serial 33, synthetic NO, window 0xe00001,
    root 0x38c, subw 0x0, time 25432718, (173,175), root:(439,464),
    state 0x10, keycode 46 (keysym 0x6c, l), same_screen YES,
    XLookupString gives 1 bytes: (6c) "l"
    XFilterEvent returns: False

*/
