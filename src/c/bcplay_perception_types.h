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
};

union bc_perception_detail {
    struct {
        struct bc_bbox not_now;  // "Not now" clickable text
    } kiosk_updated;
};

struct bc_perception {
    enum bc_glimpse glimpse;
    union bc_perception_detail detail;
};

#endif  // BCPLAY_PERCEPTION_TYPES_H_SEEN
