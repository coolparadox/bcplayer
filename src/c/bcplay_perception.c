#include <stdio.h>
#include <stdlib.h>

#include "bcplay_canvas.h"
#include "bcplay_log.h"

#include "bcplay_perception.h"

#define BC_MODULE "perception"

extern unsigned char bcpack_appsite_connect_wallet[];
extern unsigned char bcpack_appsite_connect_wallet_2[];
extern unsigned char bcpack_automatic_exit_label[];
extern unsigned char bcpack_game_characters_full_1[];
extern unsigned char bcpack_game_characters_full_2[];
extern unsigned char bcpack_game_characters_full_3[];
extern unsigned char bcpack_game_characters_title[];
extern unsigned char bcpack_game_characters_title_2[];
extern unsigned char bcpack_game_error_title[];
extern unsigned char bcpack_game_error_title_2[];
extern unsigned char bcpack_kiosk_updated_not_now[];
extern unsigned char bcpack_loading[];
extern unsigned char bcpack_metamask_unlock_button[];
extern unsigned char bcpack_metamask_unlock_mascot[];
extern unsigned char bcpack_metamask_unlocked[];
extern unsigned char bcpack_metamask_signature_request_origin[];
extern unsigned char bcpack_metamask_signature_request_sign_button[];
extern unsigned char bcpack_metamask_signature_request_title[];
extern unsigned char bcpack_game_exit[];
extern unsigned char bcpack_game_kiosk_fullscreen[];
extern unsigned char bcpack_game_kiosk_title[];
extern unsigned char bcpack_game_login_metamask_button[];
extern unsigned char bcpack_game_paused_heroes[];
extern unsigned char bcpack_game_selection_treasure_hunt[];
extern unsigned char bcpack_game_term_acceptance_checkbox[];
extern unsigned char bcpack_game_term_acceptance_button[];
extern unsigned char bcpack_unity_error[];

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

    // Metamask, signature request?
    {
        unsigned int frag_width, frag_height;
        int frag_row = -1, frag_col = -1;
        bc_canvas_unpack(bcpack_metamask_signature_request_title, frag, &frag_width, &frag_height);
        bc_canvas_fragment_map(shot, frag, frag_width, frag_height, map);
        frag_row = frag_col = -1; bc_canvas_scan_less_than(map, 0, &frag_row, &frag_col);
        if (frag_row < 0 || frag_col < 0) goto not_metamask_signature_request;
        bc_canvas_unpack(bcpack_metamask_signature_request_origin, frag, &frag_width, &frag_height);
        bc_canvas_fragment_map(shot, frag, frag_width, frag_height, map);
        frag_row = frag_col = -1; bc_canvas_scan_less_than(map, 0, &frag_row, &frag_col);
        if (frag_row < 0 || frag_col < 0) goto not_metamask_signature_request;
        bc_canvas_unpack(bcpack_metamask_signature_request_sign_button, frag, &frag_width, &frag_height);
        bc_canvas_fragment_map(shot, frag, frag_width, frag_height, map);
        frag_row = frag_col = -1; bc_canvas_scan_less_than(map, 0, &frag_row, &frag_col);
        if (frag_row < 0 || frag_col < 0) goto not_metamask_signature_request;
        struct bc_bbox* sign = &sight->detail.metamask_signature_request.sign;
        sign->tl.col = frag_col;
        sign->tl.row = frag_row;
        sign->br.col = frag_col + frag_width - 1;
        sign->br.row = frag_row + frag_height - 1;
        cleanup_return(BC_GLIMPSE_METAMASK_SIGNATURE_REQUEST, "metamask signature request");
not_metamask_signature_request:
    }

    // Metamask, unlock wallet?
    {
        unsigned int frag_width, frag_height;
        int frag_row = -1, frag_col = -1;
        bc_canvas_unpack(bcpack_metamask_unlock_mascot, frag, &frag_width, &frag_height);
        bc_canvas_fragment_map(shot, frag, frag_width, frag_height, map);
        frag_row = frag_col = -1; bc_canvas_scan_less_than(map, 0, &frag_row, &frag_col);
        if (frag_row < 0 || frag_col < 0) goto not_metamask_unlock;
        bc_canvas_unpack(bcpack_metamask_unlock_button, frag, &frag_width, &frag_height);
        bc_canvas_fragment_map(shot, frag, frag_width, frag_height, map);
        frag_row = frag_col = -1; bc_canvas_scan_less_than(map, 0, &frag_row, &frag_col);
        if (frag_row < 0 || frag_col < 0) goto not_metamask_unlock;
        struct bc_bbox* unlock = &sight->detail.metamask_unlock.unlock;
        unlock->tl.col = frag_col;
        unlock->tl.row = frag_row;
        unlock->br.col = frag_col + frag_width - 1;
        unlock->br.row = frag_row + frag_height - 1;
        cleanup_return(BC_GLIMPSE_METAMASK_UNLOCK, "metamask unlock");
not_metamask_unlock:
    }

    // Metamask, unlocked?
    {
        unsigned int frag_width, frag_height;
        int frag_row, frag_col;
        bc_canvas_unpack(bcpack_metamask_unlocked, frag, &frag_width, &frag_height);
        bc_canvas_fragment_map(shot, frag, frag_width, frag_height, map);
        frag_row = frag_col = -1; bc_canvas_scan_less_than(map, 0, &frag_row, &frag_col);
        if (frag_row >= 0 && frag_col >= 0) cleanup_return(BC_GLIMPSE_METAMASK_UNLOCKED, "metamask unlocked");
    }

    // Automatic exit error?
    {
        unsigned int frag_width, frag_height;
        int frag_row = -1, frag_col = -1;
        bc_canvas_unpack(bcpack_automatic_exit_label, frag, &frag_width, &frag_height);
        bc_canvas_fragment_map(shot, frag, frag_width, frag_height, map);
        frag_row = frag_col = -1; bc_canvas_scan_less_than(map, 0, &frag_row, &frag_col);
        if (frag_row >= 0 || frag_col >= 0) cleanup_return(BC_GLIMPSE_AUTOMATIC_EXIT, "automatic exit");
    }

    // Other type of error?
    {
        int frag_row, frag_col;
        unsigned int frag_width, frag_height;
        bc_canvas_unpack(bcpack_game_error_title, frag, &frag_width, &frag_height);
        bc_canvas_fragment_map(shot, frag, frag_width, frag_height, map);
        frag_row = frag_col = -1; bc_canvas_scan_less_than(map, 0, &frag_row, &frag_col);
        if (frag_row >= 0 && frag_col >= 0) cleanup_return(BC_GLIMPSE_ERROR_OTHER, "game error");
        bc_canvas_unpack(bcpack_game_error_title_2, frag, &frag_width, &frag_height);
        bc_canvas_fragment_map(shot, frag, frag_width, frag_height, map);
        frag_row = frag_col = -1; bc_canvas_scan_less_than(map, 0, &frag_row, &frag_col);
        if (frag_row >= 0 && frag_col >= 0) cleanup_return(BC_GLIMPSE_ERROR_OTHER, "game error");
        bc_canvas_unpack(bcpack_unity_error, frag, &frag_width, &frag_height);
        bc_canvas_fragment_map(shot, frag, frag_width, frag_height, map);
        frag_row = frag_col = -1; bc_canvas_scan_less_than(map, 0, &frag_row, &frag_col);
        if (frag_row >= 0 && frag_col >= 0) cleanup_return(BC_GLIMPSE_ERROR_OTHER, "game error");
    }

    // Game in kiosk?
    {
        unsigned int frag_width, frag_height;
        int frag_row = -1, frag_col = -1;
        bc_canvas_unpack(bcpack_game_kiosk_title, frag, &frag_width, &frag_height);
        bc_canvas_fragment_map(shot, frag, frag_width, frag_height, map);
        frag_row = frag_col = -1; bc_canvas_scan_less_than(map, 0, &frag_row, &frag_col);
        if (frag_row < 0 || frag_col < 0) goto not_game_in_kiosk;
        bc_canvas_unpack(bcpack_game_kiosk_fullscreen, frag, &frag_width, &frag_height);
        bc_canvas_fragment_map(shot, frag, frag_width, frag_height, map);
        frag_row = frag_col = -1; bc_canvas_scan_less_than(map, 0, &frag_row, &frag_col);
        if (frag_row < 0 || frag_col < 0) cleanup_return(BC_GLIMPSE_GAME_KIOSK_UNSCROLLED, "game in kiosk, unscrolled");
        struct bc_bbox* unlock = &sight->detail.game_kiosk_scrolled.fullscreen;
        unlock->tl.col = frag_col;
        unlock->tl.row = frag_row;
        unlock->br.col = frag_col + frag_width - 1;
        unlock->br.row = frag_row + frag_height - 1;
        cleanup_return(BC_GLIMPSE_GAME_KIOSK_SCROLLED, "game in kiosk, scrolled");
not_game_in_kiosk:
    }

    // App site, connect wallet?
    {
        unsigned int frag_width, frag_height;
        int frag_row, frag_col;

        bc_canvas_unpack(bcpack_appsite_connect_wallet, frag, &frag_width, &frag_height);
        bc_canvas_fragment_map(shot, frag, frag_width, frag_height, map);
        frag_row = frag_col = -1; bc_canvas_scan_less_than(map, 0, &frag_row, &frag_col);
        if (frag_row >= 0 && frag_col >= 0) {
            sight->detail.appsite_connect_wallet.connect_wallet.tl.row = frag_row;
            sight->detail.appsite_connect_wallet.connect_wallet.tl.col = frag_col;
            sight->detail.appsite_connect_wallet.connect_wallet.br.row = frag_row + frag_height - 1;
            sight->detail.appsite_connect_wallet.connect_wallet.br.col = frag_col + frag_width - 1;
            cleanup_return(BC_GLIMPSE_APPSITE_CONNECT_WALLET, "connect wallet");
        }
        bc_canvas_unpack(bcpack_appsite_connect_wallet_2, frag, &frag_width, &frag_height);
        bc_canvas_fragment_map(shot, frag, frag_width, frag_height, map);
        frag_row = frag_col = -1; bc_canvas_scan_less_than(map, 0, &frag_row, &frag_col);
        if (frag_row >= 0 && frag_col >= 0) {
            sight->detail.appsite_connect_wallet.connect_wallet.tl.row = frag_row;
            sight->detail.appsite_connect_wallet.connect_wallet.tl.col = frag_col;
            sight->detail.appsite_connect_wallet.connect_wallet.br.row = frag_row + frag_height - 1;
            sight->detail.appsite_connect_wallet.connect_wallet.br.col = frag_col + frag_width - 1;
            cleanup_return(BC_GLIMPSE_APPSITE_CONNECT_WALLET, "connect wallet");
        }
    }

    // App site, game term acceptance, selected?
    {
        unsigned int frag_width, frag_height;
        bc_canvas_unpack(bcpack_game_term_acceptance_button, frag, &frag_width, &frag_height);
        bc_canvas_fragment_map(shot, frag, frag_width, frag_height, map);
        int frag_row = -1; int frag_col = -1; bc_canvas_scan_less_than(map, 0, &frag_row, &frag_col);
        if (frag_row >= 0 && frag_col >= 0) {
            sight->detail.game_term_acceptance_unselected.checkbox.tl.row = frag_row;
            sight->detail.game_term_acceptance_unselected.checkbox.tl.col = frag_col;
            sight->detail.game_term_acceptance_unselected.checkbox.br.row = frag_row + frag_height;
            sight->detail.game_term_acceptance_unselected.checkbox.br.col = frag_col + frag_width;
            cleanup_return(BC_GLIMPSE_GAME_TERM_ACCEPTANCE_SELECTED, "term acceptance, selected");
        }
    }

    // App site, game term acceptance, unselected?
    {
        unsigned int frag_width, frag_height;
        bc_canvas_unpack(bcpack_game_term_acceptance_checkbox, frag, &frag_width, &frag_height);
        bc_canvas_fragment_map(shot, frag, frag_width, frag_height, map);
        int frag_row = -1; int frag_col = -1; bc_canvas_scan_less_than(map, 0, &frag_row, &frag_col);
        if (frag_row >= 0 && frag_col >= 0) {
            sight->detail.game_term_acceptance_unselected.checkbox.tl.row = frag_row + 6;
            sight->detail.game_term_acceptance_unselected.checkbox.tl.col = frag_col + 6;
            sight->detail.game_term_acceptance_unselected.checkbox.br.row = frag_row + 34;
            sight->detail.game_term_acceptance_unselected.checkbox.br.col = frag_col + 34;
            cleanup_return(BC_GLIMPSE_GAME_TERM_ACCEPTANCE_UNSELECTED, "term acceptance, unselected");
        }
    }

    // App site, game login with metamask?
    {
        unsigned int frag_width, frag_height;
        bc_canvas_unpack(bcpack_game_login_metamask_button, frag, &frag_width, &frag_height);
        bc_canvas_fragment_map(shot, frag, frag_width, frag_height, map);
        int frag_row = -1; int frag_col = -1; bc_canvas_scan_less_than(map, 0, &frag_row, &frag_col);
        if (frag_row >= 0 && frag_col >= 0) {
            sight->detail.game_login_with_metamask.button.tl.row = frag_row;
            sight->detail.game_login_with_metamask.button.tl.col = frag_col;
            sight->detail.game_login_with_metamask.button.br.row = frag_row + frag_height - 1;
            sight->detail.game_login_with_metamask.button.br.col = frag_col + frag_width - 1;
            cleanup_return(BC_GLIMPSE_GAME_LOGIN_WITH_METAMASK, "game login with metamask");
        }
    }

    // Game selection?
    {
        unsigned int frag_width, frag_height;
        bc_canvas_unpack(bcpack_game_selection_treasure_hunt, frag, &frag_width, &frag_height);
        bc_canvas_fragment_map(shot, frag, frag_width, frag_height, map);
        int frag_row = -1; int frag_col = -1; bc_canvas_scan_less_than(map, 0, &frag_row, &frag_col);
        if (frag_row < 0 || frag_col < 0) goto not_game_selection;
        sight->detail.game_selection.treasure_hunt.tl.row = frag_row;
        sight->detail.game_selection.treasure_hunt.tl.col = frag_col;
        sight->detail.game_selection.treasure_hunt.br.row = frag_row + frag_height - 1;
        sight->detail.game_selection.treasure_hunt.br.col = frag_col + frag_width - 1;
        cleanup_return(BC_GLIMPSE_GAME_SELECTION, "game selection");
not_game_selection:
    }

    // Character selection?
    {
        unsigned int frag_width, frag_height;
        int frag_row, frag_col;

        bc_canvas_unpack(bcpack_game_characters_title_2, frag, &frag_width, &frag_height);
        bc_canvas_fragment_map(shot, frag, frag_width, frag_height, map);
        frag_row = frag_col = -1; bc_canvas_scan_less_than(map, 0, &frag_row, &frag_col);
        if (frag_row < 0 || frag_col < 0) goto not_character_selection;
        sight->detail.game_characters.has_full = 0;

        bc_canvas_unpack(bcpack_game_characters_full_3, frag, &frag_width, &frag_height);
        bc_canvas_fragment_map(shot, frag, frag_width, frag_height, map);
        frag_row = frag_col = -1; bc_canvas_scan_less_than(map, 12, &frag_row, &frag_col);
        if (frag_row >= 0 && frag_col >= 0) {
            sight->detail.game_characters.has_full = 1;
            sight->detail.game_characters.work.tl.row = frag_row - 18;
            sight->detail.game_characters.work.tl.col = frag_col + 22;
            sight->detail.game_characters.work.br.row = frag_row + 4;
            sight->detail.game_characters.work.br.col = frag_col + 67;
            cleanup_return(BC_GLIMPSE_GAME_CHARACTERS, "character selection, full energy hero");
        }

        bc_canvas_unpack(bcpack_game_characters_full_2, frag, &frag_width, &frag_height);
        bc_canvas_fragment_map(shot, frag, frag_width, frag_height, map);
        frag_row = frag_col = -1; bc_canvas_scan_less_than(map, 12, &frag_row, &frag_col);
        if (frag_row >= 0 && frag_col >= 0) {
            sight->detail.game_characters.has_full = 1;
            sight->detail.game_characters.work.tl.row = frag_row - 18;
            sight->detail.game_characters.work.tl.col = frag_col + 22;
            sight->detail.game_characters.work.br.row = frag_row + 4;
            sight->detail.game_characters.work.br.col = frag_col + 67;
            cleanup_return(BC_GLIMPSE_GAME_CHARACTERS, "character selection, full energy hero");
        }

        bc_canvas_unpack(bcpack_game_characters_full_1, frag, &frag_width, &frag_height);
        bc_canvas_fragment_map(shot, frag, frag_width, frag_height, map);
        frag_row = frag_col = -1; bc_canvas_scan_less_than(map, 12, &frag_row, &frag_col);
        if (frag_row >= 0 && frag_col >= 0) {
            sight->detail.game_characters.has_full = 1;
            sight->detail.game_characters.work.tl.row = frag_row - 18;
            sight->detail.game_characters.work.tl.col = frag_col + 22;
            sight->detail.game_characters.work.br.row = frag_row + 4;
            sight->detail.game_characters.work.br.col = frag_col + 67;
            cleanup_return(BC_GLIMPSE_GAME_CHARACTERS, "character selection, full energy hero");
        }

        cleanup_return(BC_GLIMPSE_GAME_CHARACTERS, "character selection, no full energy hero");
not_character_selection:
    }

    // In game?
    {
        unsigned int frag_width, frag_height;
        int frag_row, frag_col;
        bc_canvas_unpack(bcpack_game_exit, frag, &frag_width, &frag_height);
        bc_canvas_fragment_map(shot, frag, frag_width, frag_height, map);
        frag_row = frag_col = -1; bc_canvas_scan_less_than(map, 0, &frag_row, &frag_col);
        if (frag_row < 0 || frag_col < 0) goto not_in_game;
        sight->detail.game_ongoing.exit.tl.row = frag_row;
        sight->detail.game_ongoing.exit.tl.col = frag_col;
        sight->detail.game_ongoing.exit.br.row = frag_row + frag_height - 1;
        sight->detail.game_ongoing.exit.br.col = frag_col + frag_width - 1;
        bc_canvas_unpack(bcpack_game_paused_heroes, frag, &frag_width, &frag_height);
        bc_canvas_fragment_map(shot, frag, frag_width, frag_height, map);
        frag_row = frag_col = -1; bc_canvas_scan_less_than(map, 0, &frag_row, &frag_col);
        if (frag_row >= 0 && frag_col >= 0) {
        sight->detail.game_paused.heroes.tl.row = frag_row;
        sight->detail.game_paused.heroes.tl.col = frag_col;
        sight->detail.game_paused.heroes.br.row = frag_row + frag_height - 1;
        sight->detail.game_paused.heroes.br.col = frag_col + frag_width - 1;
            cleanup_return(BC_GLIMPSE_GAME_PAUSED, "game paused");
        }
        cleanup_return(BC_GLIMPSE_GAME_ONGOING, "game ongoing");
not_in_game:
    }

    // Game loading?
    {
        unsigned int frag_width, frag_height;
        bc_canvas_unpack(bcpack_loading, frag, &frag_width, &frag_height);
        bc_canvas_fragment_map(shot, frag, frag_width, frag_height, map);
        int frag_row = -1; int frag_col = -1; bc_canvas_scan_less_than(map, 0, &frag_row, &frag_col);
        if (frag_row >= 0 && frag_col >= 0) cleanup_return(BC_GLIMPSE_LOADING, "game loading");
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

