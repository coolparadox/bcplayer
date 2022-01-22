#include <stdio.h>
#include <stdlib.h>

#include "bcplay_canvas.h"
#include "bcplay_log.h"

#include "bcplay_perception.h"

#define BC_MODULE "perception"

extern unsigned char bcpack_kiosk_updated_not_now[];
extern unsigned char bcpack_appsite_wrong_network[];
extern unsigned char bcpack_appsite_connect_wallet[];
extern unsigned char bcpack_metamask_unlock_mascot[];
extern unsigned char bcpack_metamask_unlock_button[];

int bc_perceive(const struct bc_canvas_pixmap* shot, struct bc_perception* sight) {

    struct bc_canvas_pixmap* frag = malloc(sizeof(struct bc_canvas_pixmap)); if (!frag) panic("cannot allocate fragment");
    struct bc_canvas_graymap* map = malloc(sizeof(struct bc_canvas_graymap)); if (!map) panic("cannot allocate map");

#define cleanup() { if (frag) free(frag); frag = NULL; if (map) free(map); map = NULL; }
#define cleanup_fail(...) { cleanup(); fail(__VA_ARGS__); }
#define cleanup_return(GLIMPSE, LOG_STR) { cleanup(); log_debug("glimpse: %s", LOG_STR); sight->glimpse = GLIMPSE; return 0; }

    // Full black?
    {
        unsigned int max = 0;
        for (unsigned int row = 0; row < BC_KIOSK_HEIGHT; ++row) for (unsigned int col = 0; col < BC_KIOSK_WIDTH; ++col) {
            const struct bc_canvas_rgb* rgb = &shot->rgb[row][col];
            if (rgb->r > max) max = rgb->r;
            if (rgb->g > max) max = rgb->g;
            if (rgb->b > max) max = rgb->b;
        }
        if (max == 0) cleanup_return(BC_GLIMPSE_BLACK, "black");
    }

    // Clean kiosk?
    {
        unsigned int private_purple_count = 0;
        for (unsigned int row = 0; row < BC_KIOSK_HEIGHT; ++row) for (unsigned int col = 0; col < BC_KIOSK_WIDTH; ++col) {
            const struct bc_canvas_rgb* rgb = &shot->rgb[row][col];
            private_purple_count += (rgb->r == 0x25 && rgb->g == 0x00 && rgb->b == 0x3e) || (rgb->r == 0x1E && rgb->g == 0x00 && rgb->b == 0x32);
        }
        if (private_purple_count >= (unsigned int) BC_KIOSK_WIDTH * BC_KIOSK_HEIGHT * 8 / 10) cleanup_return(BC_GLIMPSE_KIOSK_CLEAN, "clean kiosk");
    }

    // App site, wrong network?
    {
        unsigned int frag_width, frag_height;
        bc_canvas_unpack(bcpack_appsite_wrong_network, frag, &frag_width, &frag_height);
        bc_canvas_fragment_map(shot, frag, frag_width, frag_height, map);
        int frag_row = -1; int frag_col = -1; bc_canvas_scan_less_than(map, 0, &frag_row, &frag_col);
        if (frag_row >= 0 && frag_col >= 0) cleanup_return(BC_GLIMPSE_APPSITE_WRONG_NETWORK, "wrong network");
    }

    // Metamask, unlock wallet?
    {
        unsigned int frag_width, frag_height;
        int frag_row, frag_col;
        bc_canvas_unpack(bcpack_metamask_unlock_mascot, frag, &frag_width, &frag_height);
        bc_canvas_fragment_map(shot, frag, frag_width, frag_height, map);
        bc_canvas_scan_less_than(map, 0, &frag_row, &frag_col);
        if (frag_row < 0 || frag_col < 0) goto not_metamask_unlock;
        bc_canvas_unpack(bcpack_metamask_unlock_button, frag, &frag_width, &frag_height);
        bc_canvas_fragment_map(shot, frag, frag_width, frag_height, map);
        bc_canvas_scan_less_than(map, 0, &frag_row, &frag_col);
        if (frag_row < 0 || frag_col < 0) goto not_metamask_unlock;
        struct bc_bbox* unlock = &sight->detail.metamask_unlock.unlock;
        unlock->tl.col = frag_col;
        unlock->tl.row = frag_row;
        unlock->br.col = frag_col + frag_width - 1;
        unlock->br.row = frag_row + frag_height - 1;
        cleanup_return(BC_GLIMPSE_METAMASK_UNLOCK, "metamask unlock");
not_metamask_unlock:
    }

    // App site, connect wallet?
    {
        unsigned int frag_width, frag_height;
        bc_canvas_unpack(bcpack_appsite_connect_wallet, frag, &frag_width, &frag_height);
        bc_canvas_fragment_map(shot, frag, frag_width, frag_height, map);
        int frag_row = -1; int frag_col = -1; bc_canvas_scan_less_than(map, 0, &frag_row, &frag_col);
        if (frag_row >= 0 && frag_col >= 0) {
            sight->detail.appsite_connect_wallet.connect_wallet.tl.row = frag_row;
            sight->detail.appsite_connect_wallet.connect_wallet.tl.col = frag_col;
            sight->detail.appsite_connect_wallet.connect_wallet.br.row = frag_row + frag_height - 1;
            sight->detail.appsite_connect_wallet.connect_wallet.br.col = frag_col + frag_width - 1;
            cleanup_return(BC_GLIMPSE_APPSITE_CONNECT_WALLET, "connect wallet");
        }
    }

    // Kiosk recently updated?
    {
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
    }

    // The screenshot does not reveal anything peculiar.
    cleanup_return(BC_GLIMPSE_UNKNOWN, "unknown");

}

