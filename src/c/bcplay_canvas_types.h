#ifndef BCPLAY_CANVAS_TYPES_H_SEEN
#define BCPLAY_CANVAS_TYPES_H_SEEN

#include "bcplay_conf.h"

struct bc_canvas {
    unsigned int r[BC_KIOSK_HEIGHT][BC_KIOSK_WIDTH];
    unsigned int g[BC_KIOSK_HEIGHT][BC_KIOSK_WIDTH];
    unsigned int b[BC_KIOSK_HEIGHT][BC_KIOSK_WIDTH];
};

#endif  // BCPLAY_CANVAS_TYPES_H_SEEN
