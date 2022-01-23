#ifndef BCPLAY_PERCEPTION_TYPES_H_SEEN
#define BCPLAY_PERCEPTION_TYPES_H_SEEN

#include "bcplay_canvas_types.h"
#include "bcplay_conf.h"

enum bc_glimpse {
    BC_GLIMPSE_UNKNOWN,
    BC_GLIMPSE_BLACK,
    BC_GLIMPSE_KIOSK_UPDATED,
    BC_GLIMPSE_KIOSK_CLEAN,
    BC_GLIMPSE_APPSITE_WRONG_NETWORK,
    BC_GLIMPSE_APPSITE_CONNECT_WALLET,
    BC_GLIMPSE_METAMASK_UNLOCK,
    BC_GLIMPSE_METAMASK_SIGNATURE_REQUEST,
    BC_GLIMPSE_GAME_KIOSK_UNSCROLLED,
    BC_GLIMPSE_GAME_KIOSK_SCROLLED,
    BC_GLIMPSE_SOCKET_ERROR,
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
};

struct bc_perception {
    enum bc_glimpse glimpse;
    union bc_perception_detail detail;
};

#endif  // BCPLAY_PERCEPTION_TYPES_H_SEEN
