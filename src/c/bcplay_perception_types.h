#ifndef BCPLAY_PERCEPTION_TYPES_H_SEEN
#define BCPLAY_PERCEPTION_TYPES_H_SEEN

#include "bcplay_conf.h"
#include "bcplay_screenshot_types.h"

enum bc_glimpse {
    BC_GLIMPSE_UNKNOWN = 0,
    BC_GLIMPSE_BLACK = 1,
};

union bc_perception_detail {};

struct bc_perception {
    enum bc_glimpse glimpse;
    union bc_perception_detail detail;
};

#endif  // BCPLAY_PERCEPTION_TYPES_H_SEEN
