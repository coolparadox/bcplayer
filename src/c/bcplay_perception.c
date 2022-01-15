#include <stdio.h>

#include "bcplay_log.h"
#include "bcplay_screenshot.h"

#include "bcplay_perception.h"

#define BC_MODULE "perception"

int bc_perceive(struct bc_perception* result) {

    if (bc_screenshot_acquire(&result->screenshot)) fail("cannot acquire screenshot");
    result->glimpse = BC_GLIMPSE_UNKNOWN;
    log_debug("glimpse: unknown");
    return 0;

}

