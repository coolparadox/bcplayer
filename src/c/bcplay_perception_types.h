#ifndef BCPLAY_PERCEPTION_TYPES_H_SEEN
#define BCPLAY_PERCEPTION_TYPES_H_SEEN

#include "bcplay_conf.h"
#include "bcplay_screenshot_types.h"

enum bc_glimpse {
    BC_GLIMPSE_UNKNOWN = 0,
    BC_GLIMPSE_KIOSK = 1,
};

struct bc_perception {
    struct bc_screenshot screenshot;
    enum bc_glimpse glimpse;
    union {} detail;
};

#endif  // BCPLAY_PERCEPTION_TYPES_H_SEEN
