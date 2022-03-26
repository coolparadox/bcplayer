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

#ifndef BCPLAY_CANVAS_TYPES_H_SEEN
#define BCPLAY_CANVAS_TYPES_H_SEEN

#include "bcplay_conf.h"

struct bc_point {
    unsigned int col;
    unsigned int row;
};

struct bc_bbox {
    struct bc_point tl;  // top left
    struct bc_point br;  // bottom right
};

struct bc_canvas_rgb {
    unsigned char r, g, b;
};

struct bc_canvas_pixmap {
    struct bc_canvas_rgb rgb[BC_KIOSK_HEIGHT][BC_KIOSK_WIDTH];
};

struct bc_canvas_graymap {
    unsigned char gray[BC_KIOSK_HEIGHT][BC_KIOSK_WIDTH];
};

#endif  // BCPLAY_CANVAS_TYPES_H_SEEN
