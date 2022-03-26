/*
 * Copyright 2022 Rafael Lorandi <coolparadox@gmail.com>
 *
 * This file is part of bcplay.
 *
 * bcplay is free software: you can redistribute it and/or modify it under the terms of the GNU
 * General Public License as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * bcplay is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with bcplay. If not, see
 * <https://www.gnu.org/licenses/>.
 */

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
