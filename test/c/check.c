#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

#include <stdlib.h>

#include "bcplay_perception.h"
#include "bcplay_screenshot.h"

static void test_perceive_noise(void** state) {
    struct bc_screenshot shot = { 0 };
    for (unsigned int row = 0; row < BC_KIOSK_HEIGHT; ++row) for (unsigned int col = 0; col < BC_KIOSK_WIDTH; ++col) {
        shot.r[row][col] = rand() % 0x100;
        shot.g[row][col] = rand() % 0x100;
        shot.b[row][col] = rand() % 0x100;
    }
    struct bc_perception sight = { 0 };
    assert_false(bc_perceive(&shot, &sight));
    assert_int_equal(sight.glimpse, BC_GLIMPSE_UNKNOWN);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_perceive_noise),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}

