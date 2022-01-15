#include <stdio.h>

#include "bcplay_log.h"
#include "bcplay_screenshot.h"

#include "bcplay_perception.h"

#define BC_MODULE "perception"

int bc_perceive(const struct bc_screenshot* input, struct bc_perception* result) {
    result->glimpse = BC_GLIMPSE_UNKNOWN;
    log_debug("glimpse: unknown");
    return 0;
}

