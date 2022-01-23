#include <string.h>

#include "bcplay_conf.h"
#include "bcplay_log.h"
#include "bcplay_random.h"

#include "bcplay_planning.h"

#define BC_MODULE "planning"

extern enum bc_planning_states _bc_planning_state;
extern int _bc_planning_heroes_select;
extern int _bc_planning_gameplay_verify;

void bc_planning_init() {
    _bc_planning_state = BC_STATE_START;
    _bc_planning_heroes_select = 0;
    _bc_planning_gameplay_verify = 0;
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
    advice->sleep = 5;
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
    log_debug("advice: click button: connect wallet");
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
    log_debug("advice: type metamask password");
    struct bc_planning_hint* hint = advice->hints - 1;
    {
        (++hint)->type = BC_HINT_MOUSE_CLICK;
        hint->detail.mouse_click.coord.col = 790;
        hint->detail.mouse_click.coord.row = 340;
    }
    (++hint)->type = BC_HINT_KEYBOARD_SEQUENCE; strcpy(hint->detail.keyboard_sequence.keys, "3hdna3Ut");
    (++hint)->type = BC_HINT_KEYBOARD_CLICK; strcpy(hint->detail.keyboard_click.key, "Return");
    advice->sleep = 10;
    return 0;
}

int _bc_planning_assess_metamask_signature_request(const union bc_perception_detail* detail, struct bc_planning_recommendation* advice) {
    // Metamask awaits for permission to accept game intention to connect to the wallet.
    log_debug("advice: click button: sign");
    struct bc_planning_hint* hint = advice->hints - 1;
    {
        (++hint)->type = BC_HINT_MOUSE_CLICK;
        const struct bc_bbox* bbox = &detail->metamask_signature_request.sign;
        hint->detail.mouse_click.coord.col = bc_random_sample_uniform(bbox->tl.col, bbox->br.col);
        hint->detail.mouse_click.coord.row = bc_random_sample_uniform(bbox->tl.row, bbox->br.row);
    }
    {
        (++hint)->type = BC_HINT_MOUSE_MOVE;
        hint->detail.mouse_move.coord.col = 10;
        hint->detail.mouse_move.coord.row = 10;
    }
    advice->sleep = 5;
    return 0;
}

int _bc_planning_assess_game_kiosk_unscrolled(const union bc_perception_detail* detail, struct bc_planning_recommendation* advice) {
    // The game is inside the kiosk and the fullscreen button is not visible.
    log_debug("advice: drag scroll bars: right, down");
    struct bc_planning_hint* hint = advice->hints - 1;
    {
        (++hint)->type = BC_HINT_MOUSE_DRAG;
        hint->detail.mouse_drag.from.col = 952;
        hint->detail.mouse_drag.from.row = 22;
        hint->detail.mouse_drag.to.col = 952;
        hint->detail.mouse_drag.to.row = BC_KIOSK_HEIGHT / 2;
    }
    {
        (++hint)->type = BC_HINT_MOUSE_DRAG;
        hint->detail.mouse_drag.from.col = 6;
        hint->detail.mouse_drag.from.row = 573;
        hint->detail.mouse_drag.to.col = BC_KIOSK_WIDTH / 32;
        hint->detail.mouse_drag.to.row = 573;
    }
    advice->sleep = 0;
    return 0;
}

int _bc_planning_assess_game_kiosk_scrolled(const union bc_perception_detail* detail, struct bc_planning_recommendation* advice) {
    // The game is inside the kiosk and the fullscreen button is visible.
    log_debug("advice: click button: full screen");
    struct bc_planning_hint* hint = advice->hints - 1;
    {
        (++hint)->type = BC_HINT_MOUSE_CLICK;
        const struct bc_bbox* bbox = &detail->game_kiosk_scrolled.fullscreen;
        hint->detail.mouse_click.coord.col = bc_random_sample_uniform(bbox->tl.col, bbox->br.col);
        hint->detail.mouse_click.coord.row = bc_random_sample_uniform(bbox->tl.row, bbox->br.row);
    }
    advice->sleep = 0;
    return 0;
}

int _bc_planning_assess_socket_error(const union bc_perception_detail* detail, struct bc_planning_recommendation* advice) {
    // Seeing the dreadful 'socket error' message.
    log_debug("advice: refresh url");
    struct bc_planning_hint* hint = advice->hints - 1;
    (++hint)->type = BC_HINT_KEYBOARD_CLICK; strcpy(hint->detail.keyboard_click.key, "F5");
    advice->sleep = 30;
    return 0;
}

int _bc_planning_assess_game_selection(const union bc_perception_detail* detail, struct bc_planning_recommendation* advice) {
    // The game shows the selection of games.
    log_debug("advice: click: treasure hunt");
    struct bc_planning_hint* hint = advice->hints - 1;
    {
        (++hint)->type = BC_HINT_MOUSE_CLICK;
        const struct bc_bbox* bbox = &detail->game_selection.treasure_hunt;
        hint->detail.mouse_click.coord.col = bc_random_sample_uniform(bbox->tl.col, bbox->br.col);
        hint->detail.mouse_click.coord.row = bc_random_sample_uniform(bbox->tl.row, bbox->br.row);
    }
    advice->sleep = 5;
    return 0;
}

int _bc_planning_assess_game_ongoing(const union bc_perception_detail* detail, struct bc_planning_recommendation* advice) {
    // The game is playing righ after selecting new heroes.
    if (_bc_planning_gameplay_verify) {
        log_debug("advice: let the game play a bit");
        _bc_planning_gameplay_verify = 0;
        advice->sleep = bc_random_sample_uniform(60 * 5, 60 * 15);
        return 0;
    }
    // The game is playing for quite some time.
    log_debug("advice: click area: pause game");
    _bc_planning_heroes_select = 1;
    struct bc_planning_hint* hint = advice->hints - 1;
    {
        (++hint)->type = BC_HINT_MOUSE_CLICK;
        hint->detail.mouse_click.coord.col = bc_random_sample_uniform(467, 493);
        hint->detail.mouse_click.coord.row = bc_random_sample_uniform(568, 599);
    }
    advice->sleep = 2;
    return 0;
}

int _bc_planning_assess_game_paused(const union bc_perception_detail* detail, struct bc_planning_recommendation* advice) {
    // The game is paused.
    struct bc_planning_hint* hint = advice->hints - 1;
    if (_bc_planning_heroes_select) {
        // The pause came from the normal gameplay.
        _bc_planning_heroes_select = 0;
        log_debug("advice: click button: heroes selection");
        (++hint)->type = BC_HINT_MOUSE_CLICK;
        const struct bc_bbox* bbox = &detail->game_paused.heroes;
        hint->detail.mouse_click.coord.col = bc_random_sample_uniform(bbox->tl.col, bbox->br.col);
        hint->detail.mouse_click.coord.row = bc_random_sample_uniform(bbox->tl.row, bbox->br.row);
        advice->sleep = 3;
        return 0;
    }
    // The pause came from the character selection screen.
    log_debug("advice: click area: resume game play");
    _bc_planning_gameplay_verify = 1;
    (++hint)->type = BC_HINT_MOUSE_CLICK;
    hint->detail.mouse_click.coord.col = bc_random_sample_uniform(40, 920);
    hint->detail.mouse_click.coord.row = bc_random_sample_uniform(110, 480);
    advice->sleep = 2;
    return 0;
}

int _bc_planning_assess_game_characters(const union bc_perception_detail* detail, struct bc_planning_recommendation* advice) {
    // Character selection.
    struct bc_planning_hint* hint = advice->hints - 1;
    if (detail->game_characters.has_full) {
        log_debug("advice: click button: work");
        (++hint)->type = BC_HINT_MOUSE_CLICK;
        const struct bc_bbox* bbox = &detail->game_characters.work;
        hint->detail.mouse_click.coord.col = bc_random_sample_uniform(bbox->tl.col, bbox->br.col);
        hint->detail.mouse_click.coord.row = bc_random_sample_uniform(bbox->tl.row, bbox->br.row);
        // FIXME: add some delay here
    }
    log_debug("advice: click area: game pause");
    (++hint)->type = BC_HINT_MOUSE_CLICK;
    hint->detail.mouse_click.coord.col = bc_random_sample_uniform(276, 685);
    hint->detail.mouse_click.coord.row = bc_random_sample_uniform(566, 595);
    advice->sleep = 2;
    return 0;
}

int _bc_planning_assess_automatic_exit(const union bc_perception_detail* detail, struct bc_planning_recommendation* advice) {
    log_debug("advice: refresh url");
    // The game shows the selection of games.
    struct bc_planning_hint* hint = advice->hints - 1;
    (++hint)->type = BC_HINT_KEYBOARD_CLICK; strcpy(hint->detail.keyboard_click.key, "F5");
    advice->sleep = 10;
    return 0;
}

int bc_planning_assess(const struct bc_perception* sight, struct bc_planning_recommendation* advice) {
    memset(advice->hints, 0, BC_PLANNING_HINTS_SIZE);
    // FIXME: revert to small sample unknown waiting time
    advice->sleep = bc_random_sample_uniform(10, 20);
    //advice->sleep = 3600;
    switch (sight->glimpse) {
        case BC_GLIMPSE_UNKNOWN: return 0;
        case BC_GLIMPSE_BLACK: return _bc_planning_assess_black(&sight->detail, advice);
        case BC_GLIMPSE_KIOSK_UPDATED: log_warning("'kiosk updated' assessment not implemented"); return 0;
        case BC_GLIMPSE_KIOSK_CLEAN: return _bc_planning_assess_kiosk_clean(&sight->detail, advice);
        case BC_GLIMPSE_APPSITE_WRONG_NETWORK: return _bc_planning_assess_appsite_wrong_network(&sight->detail, advice);
        case BC_GLIMPSE_APPSITE_CONNECT_WALLET: return _bc_planning_assess_appsite_connect_wallet(&sight->detail, advice);
        case BC_GLIMPSE_METAMASK_UNLOCK: return _bc_planning_assess_metamask_unlock(&sight->detail, advice);
        case BC_GLIMPSE_METAMASK_SIGNATURE_REQUEST: return _bc_planning_assess_metamask_signature_request(&sight->detail, advice);
        case BC_GLIMPSE_GAME_KIOSK_UNSCROLLED: return _bc_planning_assess_game_kiosk_unscrolled(&sight->detail, advice);
        case BC_GLIMPSE_GAME_KIOSK_SCROLLED: return _bc_planning_assess_game_kiosk_scrolled(&sight->detail, advice);
        case BC_GLIMPSE_SOCKET_ERROR: return _bc_planning_assess_socket_error(&sight->detail, advice);
        case BC_GLIMPSE_GAME_SELECTION: return _bc_planning_assess_game_selection(&sight->detail, advice);
        case BC_GLIMPSE_GAME_ONGOING: return _bc_planning_assess_game_ongoing(&sight->detail, advice);
        case BC_GLIMPSE_GAME_PAUSED: return _bc_planning_assess_game_paused(&sight->detail, advice);
        case BC_GLIMPSE_AUTOMATIC_EXIT: return _bc_planning_assess_automatic_exit(&sight->detail, advice);
        case BC_GLIMPSE_GAME_CHARACTERS: return _bc_planning_assess_game_characters(&sight->detail, advice);
    }
    panic("unknown glimpse: %d", sight->glimpse);
}
