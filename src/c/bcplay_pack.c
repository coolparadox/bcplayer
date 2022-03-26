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

#define _GNU_SOURCE
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bcplay_canvas.h"
#include "bcplay_conf.h"

int main(int argc, char** argv) {
    assert(argc == 2);
    const char* ppm_path = argv[1];
    char* pack_name = strdup(basename(ppm_path)); pack_name[strlen(pack_name)-4] = '\0';
    struct bc_canvas_pixmap* pix = malloc(sizeof(struct bc_canvas_pixmap));
    unsigned int width, height;
    assert(!bc_canvas_load(ppm_path, pix, &width, &height));
    assert(width > 0); assert(width <= BC_KIOSK_WIDTH);
    assert(height > 0); assert(height <= BC_KIOSK_HEIGHT);
    printf("\nunsigned char bcpack_%s[] = {\n    ", pack_name);
    printf(" 0x%02X, 0x%02X,", width / 0x100, width % 0x100);
    printf(" 0x%02X, 0x%02X,", height / 0x100, height % 0x100);
    unsigned int count = 0;
    for (unsigned int row = 0; row < height; ++row) for (unsigned int col = 0; col < width; ++col) {
        if (!(count++ % 5)) printf("\n    ");
        struct bc_canvas_rgb* rgb = &pix->rgb[row][col];
        printf(" 0x%02X, 0x%02X, 0x%02X,", rgb->r, rgb->g, rgb->b);
    }
    printf("\n};\n");
    return 0;
}
