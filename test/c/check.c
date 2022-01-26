#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

#include <stdlib.h>

#include "bcplay_perception.h"
#include "bcplay_canvas.h"
#include "bcplay_conf.h"

struct bc_canvas_pixmap* shot = NULL;

static void test_perceive_loading(void** state) {
    unsigned int width, height; assert_false(bc_canvas_load("test/samples/loading_stuck.ppm", shot, &width, &height));
    assert_int_equal(width, BC_KIOSK_WIDTH); assert_int_equal(height, BC_KIOSK_HEIGHT);
    struct bc_perception sight; assert_false(bc_perceive(shot, &sight));
    assert_int_equal(sight.glimpse, BC_GLIMPSE_LOADING);
}

static void test_perceive_error_other(void** state) {
    unsigned int width, height; assert_false(bc_canvas_load("test/samples/error_wrong_network.ppm", shot, &width, &height));
    assert_int_equal(width, BC_KIOSK_WIDTH); assert_int_equal(height, BC_KIOSK_HEIGHT);
    struct bc_perception sight; assert_false(bc_perceive(shot, &sight));
    assert_int_equal(sight.glimpse, BC_GLIMPSE_ERROR_OTHER);
}

static void test_perceive_characters_no_full(void** state) {
    unsigned int width, height; assert_false(bc_canvas_load("test/samples/characters_no_full.ppm", shot, &width, &height));
    assert_int_equal(width, BC_KIOSK_WIDTH); assert_int_equal(height, BC_KIOSK_HEIGHT);
    struct bc_perception sight; assert_false(bc_perceive(shot, &sight));
    assert_int_equal(sight.glimpse, BC_GLIMPSE_GAME_CHARACTERS);
    assert_int_equal(sight.detail.game_characters.has_full, 0);
}

static void test_perceive_characters_full_unselected(void** state) {
    unsigned int width, height; assert_false(bc_canvas_load("test/samples/characters_full_unselected.ppm", shot, &width, &height));
    assert_int_equal(width, BC_KIOSK_WIDTH); assert_int_equal(height, BC_KIOSK_HEIGHT);
    struct bc_perception sight; assert_false(bc_perceive(shot, &sight));
    assert_int_equal(sight.glimpse, BC_GLIMPSE_GAME_CHARACTERS);
    assert_int_equal(sight.detail.game_characters.has_full, 1);
    assert_int_equal(sight.detail.game_characters.work.tl.row, 331);
    assert_int_equal(sight.detail.game_characters.work.tl.col, 373);
    assert_int_equal(sight.detail.game_characters.work.br.row, 361);
    assert_int_equal(sight.detail.game_characters.work.br.col, 423);
}

static void test_perceive_characters_full_selected(void** state) {
    unsigned int width, height; assert_false(bc_canvas_load("test/samples/characters_full_selected.ppm", shot, &width, &height));
    assert_int_equal(width, BC_KIOSK_WIDTH); assert_int_equal(height, BC_KIOSK_HEIGHT);
    struct bc_perception sight; assert_false(bc_perceive(shot, &sight));
    assert_int_equal(sight.glimpse, BC_GLIMPSE_GAME_CHARACTERS);
    assert_int_equal(sight.detail.game_characters.has_full, 1);
    assert_int_equal(sight.detail.game_characters.work.tl.row, 331);
    assert_int_equal(sight.detail.game_characters.work.tl.col, 373);
    assert_int_equal(sight.detail.game_characters.work.br.row, 361);
    assert_int_equal(sight.detail.game_characters.work.br.col, 423);
}

static void test_perceive_automatic_exit(void** state) {
    unsigned int width, height; assert_false(bc_canvas_load("test/samples/error_automatic_exit.ppm", shot, &width, &height));
    assert_int_equal(width, BC_KIOSK_WIDTH); assert_int_equal(height, BC_KIOSK_HEIGHT);
    struct bc_perception sight; assert_false(bc_perceive(shot, &sight));
    assert_int_equal(sight.glimpse, BC_GLIMPSE_AUTOMATIC_EXIT);
}

static void test_perceive_game_paused(void** state) {
    unsigned int width, height; assert_false(bc_canvas_load("test/samples/game_paused.ppm", shot, &width, &height));
    assert_int_equal(width, BC_KIOSK_WIDTH); assert_int_equal(height, BC_KIOSK_HEIGHT);
    struct bc_perception sight; assert_false(bc_perceive(shot, &sight));
    assert_int_equal(sight.glimpse, BC_GLIMPSE_GAME_PAUSED);
    assert_int_equal(sight.detail.game_paused.heroes.tl.row, 576);
    assert_int_equal(sight.detail.game_paused.heroes.tl.col, 446);
    assert_int_equal(sight.detail.game_paused.heroes.br.row, 592);
    assert_int_equal(sight.detail.game_paused.heroes.br.col, 515);
}

static void test_perceive_game_ongoing(void** state) {
    unsigned int width, height; assert_false(bc_canvas_load("test/samples/game_ongoing.ppm", shot, &width, &height));
    assert_int_equal(width, BC_KIOSK_WIDTH); assert_int_equal(height, BC_KIOSK_HEIGHT);
    struct bc_perception sight; assert_false(bc_perceive(shot, &sight));
    assert_int_equal(sight.glimpse, BC_GLIMPSE_GAME_ONGOING);
}

static void test_perceive_game_selection(void** state) {
    unsigned int width, height; assert_false(bc_canvas_load("test/samples/game_selection.ppm", shot, &width, &height));
    assert_int_equal(width, BC_KIOSK_WIDTH); assert_int_equal(height, BC_KIOSK_HEIGHT);
    struct bc_perception sight; assert_false(bc_perceive(shot, &sight));
    assert_int_equal(sight.glimpse, BC_GLIMPSE_GAME_SELECTION);
    assert_int_equal(sight.detail.game_selection.treasure_hunt.tl.row, 415);
    assert_int_equal(sight.detail.game_selection.treasure_hunt.tl.col, 390);
    assert_int_equal(sight.detail.game_selection.treasure_hunt.br.row, 433);
    assert_int_equal(sight.detail.game_selection.treasure_hunt.br.col, 591);
}

static void test_perceive_game_kiosk_scrolled(void** state) {
    unsigned int width, height; assert_false(bc_canvas_load("test/samples/game_kiosk_scrolled.ppm", shot, &width, &height));
    assert_int_equal(width, BC_KIOSK_WIDTH); assert_int_equal(height, BC_KIOSK_HEIGHT);
    struct bc_perception sight; assert_false(bc_perceive(shot, &sight));
    assert_int_equal(sight.glimpse, BC_GLIMPSE_GAME_KIOSK_SCROLLED);
    assert_int_equal(sight.detail.game_kiosk_scrolled.fullscreen.tl.row, 436);
    assert_int_equal(sight.detail.game_kiosk_scrolled.fullscreen.tl.col, 825);
    assert_int_equal(sight.detail.game_kiosk_scrolled.fullscreen.br.row, 473);
    assert_int_equal(sight.detail.game_kiosk_scrolled.fullscreen.br.col, 862);
}

static void test_perceive_game_kiosk_unscrolled(void** state) {
    unsigned int width, height; assert_false(bc_canvas_load("test/samples/game_kiosk_unscrolled.ppm", shot, &width, &height));
    assert_int_equal(width, BC_KIOSK_WIDTH); assert_int_equal(height, BC_KIOSK_HEIGHT);
    struct bc_perception sight; assert_false(bc_perceive(shot, &sight));
    assert_int_equal(sight.glimpse, BC_GLIMPSE_GAME_KIOSK_UNSCROLLED);
}

static void test_perceive_metamask_signature_request(void** state) {
    unsigned int width, height; assert_false(bc_canvas_load("test/samples/metamask_signature_request.ppm", shot, &width, &height));
    assert_int_equal(width, BC_KIOSK_WIDTH); assert_int_equal(height, BC_KIOSK_HEIGHT);
    struct bc_perception sight; assert_false(bc_perceive(shot, &sight));
    assert_int_equal(sight.glimpse, BC_GLIMPSE_METAMASK_SIGNATURE_REQUEST);
    assert_int_equal(sight.detail.metamask_unlock.unlock.tl.row, 579);
    assert_int_equal(sight.detail.metamask_unlock.unlock.tl.col, 843);
    assert_int_equal(sight.detail.metamask_unlock.unlock.br.row, 593);
    assert_int_equal(sight.detail.metamask_unlock.unlock.br.col, 872);
}

static void test_perceive_metamask_unlock(void** state) {
    unsigned int width, height; assert_false(bc_canvas_load("test/samples/metamask_unlock.ppm", shot, &width, &height));
    assert_int_equal(width, BC_KIOSK_WIDTH); assert_int_equal(height, BC_KIOSK_HEIGHT);
    struct bc_perception sight; assert_false(bc_perceive(shot, &sight));
    assert_int_equal(sight.glimpse, BC_GLIMPSE_METAMASK_UNLOCK);
    assert_int_equal(sight.detail.metamask_unlock.unlock.tl.row, 406);
    assert_int_equal(sight.detail.metamask_unlock.unlock.tl.col, 756);
    assert_int_equal(sight.detail.metamask_unlock.unlock.br.row, 419);
    assert_int_equal(sight.detail.metamask_unlock.unlock.br.col, 804);
}

static void test_perceive_appsite_connect_wallet(void** state) {
    unsigned int width, height; assert_false(bc_canvas_load("test/samples/appsite_connect_wallet.ppm", shot, &width, &height));
    assert_int_equal(width, BC_KIOSK_WIDTH); assert_int_equal(height, BC_KIOSK_HEIGHT);
    struct bc_perception sight; assert_false(bc_perceive(shot, &sight));
    assert_int_equal(sight.glimpse, BC_GLIMPSE_APPSITE_CONNECT_WALLET);
    assert_int_equal(sight.detail.appsite_connect_wallet.connect_wallet.tl.row, 486);
    assert_int_equal(sight.detail.appsite_connect_wallet.connect_wallet.tl.col, 418);
    assert_int_equal(sight.detail.appsite_connect_wallet.connect_wallet.br.row, 553);
    assert_int_equal(sight.detail.appsite_connect_wallet.connect_wallet.br.col, 588);
}

static void test_perceive_kiosk_clean(void** state) {
    unsigned int width, height; assert_false(bc_canvas_load("test/samples/kiosk_clean.ppm", shot, &width, &height));
    assert_int_equal(width, BC_KIOSK_WIDTH); assert_int_equal(height, BC_KIOSK_HEIGHT);
    struct bc_perception sight; assert_false(bc_perceive(shot, &sight));
    assert_int_equal(sight.glimpse, BC_GLIMPSE_KIOSK_CLEAN);
}

static void test_perceive_kiosk_updated(void** state) {
    unsigned int width, height; assert_false(bc_canvas_load("test/samples/kiosk_updated.ppm", shot, &width, &height));
    assert_int_equal(width, BC_KIOSK_WIDTH); assert_int_equal(height, BC_KIOSK_HEIGHT);
    struct bc_perception sight; assert_false(bc_perceive(shot, &sight));
    assert_int_equal(sight.glimpse, BC_GLIMPSE_KIOSK_UPDATED);
    assert_int_equal(sight.detail.kiosk_updated.not_now.tl.row, 452);
    assert_int_equal(sight.detail.kiosk_updated.not_now.tl.col, 450);
    assert_int_equal(sight.detail.kiosk_updated.not_now.br.row, 463);
    assert_int_equal(sight.detail.kiosk_updated.not_now.br.col, 510);
}

static void test_perceive_black(void** state) {
    for (unsigned int row = 0; row < BC_KIOSK_HEIGHT; ++row) for (unsigned int col = 0; col < BC_KIOSK_WIDTH; ++col) {
        struct bc_canvas_rgb* rgb = &shot->rgb[row][col];
        rgb->r = rgb->g = rgb->b = 0x00;
    }
    struct bc_perception sight; assert_false(bc_perceive(shot, &sight));
    assert_int_equal(sight.glimpse, BC_GLIMPSE_BLACK);
}

static void test_perceive_noise(void** state) {
    for (unsigned int row = 0; row < BC_KIOSK_HEIGHT; ++row) for (unsigned int col = 0; col < BC_KIOSK_WIDTH; ++col) {
        struct bc_canvas_rgb* rgb = &shot->rgb[row][col];
        rgb->r = rand() % 0x100;
        rgb->g = rand() % 0x100;
        rgb->b = rand() % 0x100;
    }
    struct bc_perception sight; assert_false(bc_perceive(shot, &sight));
    assert_int_equal(sight.glimpse, BC_GLIMPSE_UNKNOWN);
}

int main(void) {
    setlogmask(LOG_UPTO(LOG_DEBUG));
    openlog(NULL, LOG_PERROR, LOG_LOCAL0);
    shot = malloc(sizeof(struct bc_canvas_pixmap));
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_perceive_loading),
        cmocka_unit_test(test_perceive_error_other),
        cmocka_unit_test(test_perceive_characters_no_full),
        cmocka_unit_test(test_perceive_characters_full_unselected),
        cmocka_unit_test(test_perceive_characters_full_selected),
        cmocka_unit_test(test_perceive_automatic_exit),
        cmocka_unit_test(test_perceive_game_paused),
        cmocka_unit_test(test_perceive_game_ongoing),
        cmocka_unit_test(test_perceive_game_selection),
        cmocka_unit_test(test_perceive_game_kiosk_scrolled),
        cmocka_unit_test(test_perceive_game_kiosk_unscrolled),
        cmocka_unit_test(test_perceive_metamask_signature_request),
        cmocka_unit_test(test_perceive_metamask_unlock),
        cmocka_unit_test(test_perceive_appsite_connect_wallet),
        cmocka_unit_test(test_perceive_kiosk_clean),
        cmocka_unit_test(test_perceive_kiosk_updated),
        cmocka_unit_test(test_perceive_black),
        cmocka_unit_test(test_perceive_noise),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}

