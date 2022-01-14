#include <stdio.h>

#ifndef BCPLAY_SCREENSHOT_H_SEEN
#define BCPLAY_SCREENSHOT_H_SEEN

#include "bcplay_screenshot_types.h"

int bc_screenshot_dump(const char* ppm_path, const struct bc_screenshot* from);

const char* bc_screenshot_make_dump(const struct bc_screenshot* from);

int bc_screenshot_restore(const char* ppm_path, struct bc_screenshot* to);

int bc_screenshot_acquire(struct bc_screenshot* to);

#endif  // BCPLAY_SCREENSHOT_H_SEEN
