#include <stdio.h>

#ifndef BCPLAY_SCREENSHOT_H_SEEN
#define BCPLAY_SCREENSHOT_H_SEEN

#include "bcplay_canvas_types.h"

int bc_screenshot_acquire(struct bc_canvas* to);

const char* bc_screenshot_make_dump(const struct bc_canvas* from);

int bc_screenshot_dump(const char* ppm_path, const struct bc_canvas* from);

int bc_screenshot_restore(const char* ppm_path, struct bc_canvas* to);

#endif  // BCPLAY_SCREENSHOT_H_SEEN
