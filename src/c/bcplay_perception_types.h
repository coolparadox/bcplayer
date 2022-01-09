#ifndef BCPLAY_PERCEPTION_TYPES_H_SEEN
#define BCPLAY_PERCEPTION_TYPES_H_SEEN

#include "bcplay_conf.h"

struct bc_screenshot {
    unsigned int r[BC_KIOSK_HEIGHT][BC_KIOSK_WIDTH];
    unsigned int g[BC_KIOSK_HEIGHT][BC_KIOSK_WIDTH];
    unsigned int b[BC_KIOSK_HEIGHT][BC_KIOSK_WIDTH];
};

enum bc_glimpse {
    BC_GLIMPSE_UNKNOWN,
    BC_GLIMPSE_KIOSK,  // The kiosk first appearance.
};

struct bc_perception {
    struct bc_screenshot screenshot;
    enum bc_glimpse glimpse;
    union {} detail;
};

#endif  // BCPLAY_PERCEPTION_TYPES_H_SEEN
