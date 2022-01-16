#include <stdio.h>

#include "bcplay_log.h"
#include "bcplay_screenshot.h"

#include "bcplay_perception.h"

#define BC_MODULE "perception"

int bc_perceive(const struct bc_canvas* shot, struct bc_perception* sight) {

    unsigned int max = 0;
    for (unsigned int row = 0; row < BC_KIOSK_HEIGHT; ++row) for (unsigned int col = 0; col < BC_KIOSK_WIDTH; ++col) {
        unsigned int r = shot->r[row][col];
        unsigned int g = shot->g[row][col];
        unsigned int b = shot->b[row][col];
        if (r > max) max = r;
        if (g > max) max = g;
        if (b > max) max = b;
    }
    if (max == 0) { log_debug("glimpse: black"); sight->glimpse = BC_GLIMPSE_BLACK; return 0; }

    sight->glimpse = BC_GLIMPSE_UNKNOWN;
    log_debug("glimpse: unknown");
    return 0;
}

