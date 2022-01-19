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

static void test_perceive_black(void** state) {
    for (unsigned int row = 0; row < BC_KIOSK_HEIGHT; ++row) for (unsigned int col = 0; col < BC_KIOSK_WIDTH; ++col) {
        shot->red[row][col] = 0x00;
        shot->green[row][col] = 0x00;
        shot->blue[row][col] = 0x00;
    }
    struct bc_perception sight; assert_false(bc_perceive(shot, &sight));
    assert_int_equal(sight.glimpse, BC_GLIMPSE_BLACK);
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

static void test_perceive_kiosk_clean(void** state) {
    unsigned int width, height; assert_false(bc_canvas_load("test/samples/kiosk_clean.ppm", shot, &width, &height));
    assert_int_equal(width, BC_KIOSK_WIDTH); assert_int_equal(height, BC_KIOSK_HEIGHT);
    struct bc_perception sight; assert_false(bc_perceive(shot, &sight));
    assert_int_equal(sight.glimpse, BC_GLIMPSE_KIOSK_CLEAN);
}

static void test_perceive_noise(void** state) {
    for (unsigned int row = 0; row < BC_KIOSK_HEIGHT; ++row) for (unsigned int col = 0; col < BC_KIOSK_WIDTH; ++col) {
        shot->red[row][col] = rand() % 0x100;
        shot->green[row][col] = rand() % 0x100;
        shot->blue[row][col] = rand() % 0x100;
    }
    struct bc_perception sight; assert_false(bc_perceive(shot, &sight));
    assert_int_equal(sight.glimpse, BC_GLIMPSE_UNKNOWN);
}

int main(void) {
    setlogmask(LOG_UPTO(LOG_DEBUG));
    openlog(NULL, LOG_PERROR, LOG_LOCAL0);
    shot = malloc(sizeof(struct bc_canvas_pixmap));
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_perceive_black),
        cmocka_unit_test(test_perceive_kiosk_updated),
        cmocka_unit_test(test_perceive_kiosk_clean),
        cmocka_unit_test(test_perceive_noise),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}

