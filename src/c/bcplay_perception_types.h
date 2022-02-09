#ifndef BCPLAY_PERCEPTION_TYPES_H_SEEN
#define BCPLAY_PERCEPTION_TYPES_H_SEEN

#include "bcplay_canvas_types.h"
#include "bcplay_conf.h"

enum bc_glimpse {
    BC_GLIMPSE_UNKNOWN,
    BC_GLIMPSE_BLACK,
    BC_GLIMPSE_KIOSK_UPDATED,
    BC_GLIMPSE_KIOSK_CLEAN,
    BC_GLIMPSE_APPSITE_CONNECT_WALLET,
    BC_GLIMPSE_METAMASK_UNLOCK,
    BC_GLIMPSE_METAMASK_SIGNATURE_REQUEST,
    BC_GLIMPSE_GAME_KIOSK_UNSCROLLED,
    BC_GLIMPSE_GAME_KIOSK_SCROLLED,
    BC_GLIMPSE_GAME_SELECTION,
    BC_GLIMPSE_GAME_ONGOING,
    BC_GLIMPSE_GAME_PAUSED,
    BC_GLIMPSE_AUTOMATIC_EXIT,
    BC_GLIMPSE_GAME_CHARACTERS,
    BC_GLIMPSE_ERROR_OTHER,
    BC_GLIMPSE_LOADING,
};

union bc_perception_detail {
    struct {
        struct bc_bbox not_now;  // "Not now" clickable text
    } kiosk_updated;
    struct {
        struct bc_bbox connect_wallet;  // "Connect wallet" clickable text
    } appsite_connect_wallet;
    struct {
        struct bc_bbox unlock;  // "Unlock" clickable text
    } metamask_unlock;
    struct {
        struct bc_bbox sign;  // "Sign" clickable text
    } metamask_signature_request;
    struct {
        struct bc_bbox fullscreen;  // Fullscreen button
    } game_kiosk_scrolled;
    struct {
        struct bc_bbox treasure_hunt;  // "treasure hunt" clickable text
    } game_selection;
    struct {
        struct bc_bbox exit;  // game exit button
    } game_ongoing;
    struct {
        struct bc_bbox heroes;  // "heroes" clickable text
    } game_paused;
    struct {
        int has_full;  // has a full energy character?
        struct bc_bbox work;  // "work" clickable text if has_full
    } game_characters;
};

struct bc_perception {
    enum bc_glimpse glimpse;
    union bc_perception_detail detail;
};

#endif  // BCPLAY_PERCEPTION_TYPES_H_SEEN
