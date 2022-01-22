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

int _bc_planning_assess_black(const union bc_perception_detail* detail, struct bc_planning_recommendation* advice) {
    // The vnc has just started.
    log_debug("advice: just wait (for the kiosk window)");
    advice->sleep = 2;
    return 0;
}

int _bc_planning_assess_kiosk_clean(const union bc_perception_detail* detail, struct bc_planning_recommendation* advice) {
    // The kiosk browser has just appeared.
    log_debug("advice: type the game url");
    struct bc_planning_hint* hint = advice->hints - 1;
    (++hint)->type = BC_HINT_KEYBOARD_CLICK; strcpy(hint->detail.keyboard_click.key, "ctrl+l");
    (++hint)->type = BC_HINT_KEYBOARD_SEQUENCE; strcpy(hint->detail.keyboard_sequence.keys, "app.bombcrypto.io");
    (++hint)->type = BC_HINT_KEYBOARD_CLICK; strcpy(hint->detail.keyboard_click.key, "Return");
    advice->sleep = 10;
    return 0;
}

int _bc_planning_assess_appsite_wrong_network(const union bc_perception_detail* detail, struct bc_planning_recommendation* advice) {
    // App site reports wrong network.
    log_debug("advice: refresh url");
    struct bc_planning_hint* hint = advice->hints - 1;
    (++hint)->type = BC_HINT_KEYBOARD_CLICK; strcpy(hint->detail.keyboard_click.key, "F5");
    advice->sleep = 10;
    return 0;
}

int _bc_planning_assess_appsite_connect_wallet(const union bc_perception_detail* detail, struct bc_planning_recommendation* advice) {
    // App site awaits user for connecting to the wallet.
    struct bc_planning_hint* hint = advice->hints - 1;
    {
        (++hint)->type = BC_HINT_MOUSE_CLICK;
        const struct bc_bbox* bbox = &detail->appsite_connect_wallet.connect_wallet;
        hint->detail.mouse_click.coord.col = bc_random_sample_uniform(bbox->tl.col, bbox->br.col);
        hint->detail.mouse_click.coord.row = bc_random_sample_uniform(bbox->tl.row, bbox->br.row);
    }
    {
        (++hint)->type = BC_HINT_MOUSE_MOVE;
        hint->detail.mouse_move.coord.col = 10;
        hint->detail.mouse_move.coord.row = 10;
    }
    advice->sleep = 10;
    return 0;
}

int _bc_planning_assess_metamask_unlock(const union bc_perception_detail* detail, struct bc_planning_recommendation* advice) {
    // Metamask awaits for the unlock password.
    struct bc_planning_hint* hint = advice->hints - 1;
    (++hint)->type = BC_HINT_KEYBOARD_SEQUENCE; strcpy(hint->detail.keyboard_sequence.keys, "3hdna3Ut");
    (++hint)->type = BC_HINT_KEYBOARD_CLICK; strcpy(hint->detail.keyboard_click.key, "Return");
    advice->sleep = 10;
    return 0;
}

int bc_planning_assess(const struct bc_perception* sight, struct bc_planning_recommendation* advice) {
    memset(advice->hints, 0, BC_PLANNING_HINTS_SIZE);
    advice->sleep = 3600;
    switch (sight->glimpse) {
        case BC_GLIMPSE_UNKNOWN: advice->sleep = bc_random_sample_uniform(10, 20); return 0;
        case BC_GLIMPSE_BLACK: return _bc_planning_assess_black(&sight->detail, advice);
        case BC_GLIMPSE_KIOSK_UPDATED: log_warning("'kiosk updated' assessment not implemented"); return 0;
        case BC_GLIMPSE_KIOSK_CLEAN: return _bc_planning_assess_kiosk_clean(&sight->detail, advice);
        case BC_GLIMPSE_APPSITE_WRONG_NETWORK: return _bc_planning_assess_appsite_wrong_network(&sight->detail, advice);
        case BC_GLIMPSE_APPSITE_CONNECT_WALLET: return _bc_planning_assess_appsite_connect_wallet(&sight->detail, advice);
        case BC_GLIMPSE_METAMASK_UNLOCK: return _bc_planning_assess_metamask_unlock(&sight->detail, advice);
    }
    panic("unknown glimpse: %d", sight->glimpse);
}
