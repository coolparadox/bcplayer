#ifndef BCPLAY_CANVAS_TYPES_H_SEEN
#define BCPLAY_CANVAS_TYPES_H_SEEN

#include "bcplay_conf.h"

struct bc_point {
    unsigned int col;
    unsigned int row;
};

struct bc_bbox {
    struct bc_point tl;  // top left
    struct bc_point br;  // bottom right
};

struct bc_canvas_pixmap {
    unsigned int red[BC_KIOSK_HEIGHT][BC_KIOSK_WIDTH];
    unsigned int green[BC_KIOSK_HEIGHT][BC_KIOSK_WIDTH];
    unsigned int blue[BC_KIOSK_HEIGHT][BC_KIOSK_WIDTH];
};

struct bc_canvas_graymap {
    unsigned int gray[BC_KIOSK_HEIGHT][BC_KIOSK_WIDTH];
};

struct bc_canvas_bitmap {
    unsigned int bit[BC_KIOSK_HEIGHT][BC_KIOSK_WIDTH];
};

#endif  // BCPLAY_CANVAS_TYPES_H_SEEN
