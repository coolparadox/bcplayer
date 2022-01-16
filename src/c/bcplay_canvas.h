#ifndef BCPLAY_CANVAS_H_SEEN
#define BCPLAY_CANVAS_H_SEEN

#include "bcplay_canvas_types.h"

const char* bc_canvas_make_dump(const struct bc_canvas_pixmap* from);

int bc_canvas_save(const char* path, const struct bc_canvas_pixmap* from);

int bc_canvas_load(const char* path, struct bc_canvas_pixmap* to, unsigned int* width, unsigned int* height);

void bc_canvas_fragment_map(const struct bc_canvas_pixmap* base, const struct bc_canvas_pixmap* frag, unsigned int frag_width, unsigned int frag_height, struct bc_canvas_graymap* result);

void bc_canvas_scan_less_than(const struct bc_canvas_graymap* canvas, unsigned int threshold, int* row, int* col);

void bc_canvas_unpack(unsigned char* from, struct bc_canvas_pixmap* to, unsigned int* width, unsigned int* height);

#endif  // BCPLAY_CANVAS_H_SEEN
