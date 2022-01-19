#include <stdio.h>
#include <stdlib.h>

#include "bcplay_canvas.h"
#include "bcplay_log.h"

#include "bcplay_perception.h"

#define BC_MODULE "perception"

extern unsigned char bcpack_kiosk_updated_not_now[];

int bc_perceive(const struct bc_canvas_pixmap* shot, struct bc_perception* sight) {

    struct bc_canvas_pixmap* frag = malloc(sizeof(struct bc_canvas_pixmap)); if (!frag) panic("cannot allocate fragment");
    struct bc_canvas_graymap* map = malloc(sizeof(struct bc_canvas_graymap)); if (!map) panic("cannot allocate map");

#define cleanup() { if (frag) free(frag); frag = NULL; if (map) free(map); map = NULL; }
#define cleanup_fail(...) { cleanup(); fail(__VA_ARGS__); }
#define cleanup_return(GLIMPSE, LOG_STR) { cleanup(); log_debug("glimpse: %s", LOG_STR); sight->glimpse = GLIMPSE; return 0; }

    // Full black?
    unsigned int max = 0;
    for (unsigned int row = 0; row < BC_KIOSK_HEIGHT; ++row) for (unsigned int col = 0; col < BC_KIOSK_WIDTH; ++col) {
        unsigned int r = shot->red[row][col];
        unsigned int g = shot->green[row][col];
        unsigned int b = shot->blue[row][col];
        if (r > max) max = r;
        if (g > max) max = g;
        if (b > max) max = b;
    }
    if (max == 0) cleanup_return(BC_GLIMPSE_BLACK, "black");

    // Kiosk recently updated?
    unsigned int frag_width, frag_height;
    bc_canvas_unpack(bcpack_kiosk_updated_not_now, frag, &frag_width, &frag_height);
    bc_canvas_fragment_map(shot, frag, frag_width, frag_height, map);
    int frag_row = -1; int frag_col = -1; bc_canvas_scan_less_than(map, 0, &frag_row, &frag_col);
    if (frag_row >= 0 && frag_col >= 0) {
        sight->detail.kiosk_updated.not_now.tl.row = frag_row + 1;
        sight->detail.kiosk_updated.not_now.tl.col = frag_col + 1;
        sight->detail.kiosk_updated.not_now.br.row = frag_row + frag_height - 2;
        sight->detail.kiosk_updated.not_now.br.col = frag_col + frag_width - 2;
        cleanup_return(BC_GLIMPSE_KIOSK_UPDATED, "updated kiosk");
    }

    // Clean kiosk?
    unsigned int private_purple_count = 0;
    for (unsigned int row = 0; row < BC_KIOSK_HEIGHT; ++row) for (unsigned int col = 0; col < BC_KIOSK_WIDTH; ++col) {
        unsigned int r = shot->red[row][col];
        unsigned int g = shot->green[row][col];
        unsigned int b = shot->blue[row][col];
        private_purple_count += (r == 0x25 && g == 0x00 && b == 0x3e) || (r == 0x1E && g == 0x00 && b == 0x32);
    }
    log_debug("purple count: %u", private_purple_count);
    if (private_purple_count >= (unsigned int) BC_KIOSK_WIDTH * BC_KIOSK_HEIGHT * 8 / 10) cleanup_return(BC_GLIMPSE_KIOSK_CLEAN, "clean kiosk");

    // The screenshot does not reveal anything peculiar.
    cleanup_return(BC_GLIMPSE_UNKNOWN, "unknown");

}

