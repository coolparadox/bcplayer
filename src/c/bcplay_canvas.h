#ifndef BCPLAY_CANVAS_H_SEEN
#define BCPLAY_CANVAS_H_SEEN

#include "bcplay_canvas_types.h"

const char* bc_canvas_make_dump(const struct bc_canvas* from);

int bc_canvas_dump(const char* ppm_path, const struct bc_canvas* from);

int bc_canvas_restore(const char* ppm_path, struct bc_canvas* to);

#endif  // BCPLAY_CANVAS_H_SEEN
