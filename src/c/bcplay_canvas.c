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

#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <time.h>

#include "bcplay_log.h"

#include "bcplay_canvas.h"

#define BC_MODULE "canvas"

extern char _bc_canvas_path[BC_PATH_MAX];

const char* bc_canvas_make_dump(const struct bc_canvas_pixmap* from) {
    snprintf(_bc_canvas_path, BC_PATH_MAX, "%s/screenshot_%lX.ppm", BC_TRACE_DIR, time(NULL));
    if (bc_canvas_save(_bc_canvas_path, from)) return "<error>";
    return _bc_canvas_path;
}

#define close_fail(...) { if (file) fclose(file); fail(__VA_ARGS__); }

int bc_canvas_save(const char* path, const struct bc_canvas_pixmap* from) {
    // http://netpbm.sourceforge.net/doc/ppm.html
    FILE* file = fopen(path, "w"); if (!file) close_fail("cannot open '%s' for writing: %m", path);
    if (fprintf(file, "P6\n%s\n%u %u\n255\n", "# Produced by bcplay <coolparadox@gmail.com>", BC_KIOSK_WIDTH, BC_KIOSK_HEIGHT) < 0) close_fail("cannot write ppm header to '%s'", path);
    for (unsigned int row = 0; row < BC_KIOSK_HEIGHT; ++row) for (unsigned int col = 0; col < BC_KIOSK_WIDTH; ++col) {
        const struct bc_canvas_rgb* rgb = &from->rgb[row][col];
        if (fputc(rgb->r, file) < 0) close_fail("cannot write ppm data to '%s'", path);
        if (fputc(rgb->g, file) < 0) close_fail("cannot write ppm data to '%s'", path);
        if (fputc(rgb->b, file) < 0) close_fail("cannot write ppm data to '%s'", path);
    }
    if (fclose(file)) fail("cannot close '%s': %m", path);
    return 0;
}

int _bc_canvas_load_parse_header(FILE* file);
int _bc_canvas_load_parse_comment(FILE* file);
int _bc_canvas_load_parse_number(FILE* file, unsigned int* result);

int bc_canvas_load(const char* path, struct bc_canvas_pixmap* to, unsigned int* width, unsigned int* height) {
    // File format reference: http://netpbm.sourceforge.net/doc/ppm.html
    // Tested with binary ppm files generated by GIMP.
    FILE* file = fopen(path, "r"); if (!file) fail("cannot open '%s' for reading: %m", path);
    if (_bc_canvas_load_parse_header(file)) close_fail("unrecognized file format: '%s'", path);
    if (_bc_canvas_load_parse_comment(file)) close_fail("cannot parse ppm header comment: '%s'", path);
    {
        if (_bc_canvas_load_parse_number(file, width)) close_fail("cannot parse ppm width: '%s'", path);
        if (*width > BC_KIOSK_WIDTH) close_fail("unsupported ppm width %d: %s", *width, path);
    }
    {
        if (_bc_canvas_load_parse_number(file, height)) close_fail("cannot parse ppm height: '%s'", path);
        if (*height > BC_KIOSK_HEIGHT) close_fail("unsupported ppm height %d: %s", *height, path);
    }
    {
        unsigned int max; if (_bc_canvas_load_parse_number(file, &max)) close_fail("cannot parse ppm max value: '%s'", path);
        if (max != 0xFF) close_fail("unsupported ppm max value %d: %s", max, path);
    }
    for (unsigned int row = 0; row < *height; ++row) for (unsigned int col = 0; col < *width; ++col) {
        struct bc_canvas_rgb* rgb = &to->rgb[row][col];
        {
            int c;
            if ((c = fgetc(file)) < 0) close_fail("cannot read ppm data from '%s'", path); rgb->r = c;
            if ((c = fgetc(file)) < 0) close_fail("cannot read ppm data from '%s'", path); rgb->g = c;
            if ((c = fgetc(file)) < 0) close_fail("cannot read ppm data from '%s'", path); rgb->b = c;
        }
    }
    if(fclose(file)) fail("cannot close '%s': %m", path);
    return 0;
}

int _bc_canvas_load_parse_number(FILE* file, unsigned int* result) {
    unsigned long long int n = 0;
    while (1) {
        int c = fgetc(file); if (c < 0) return 1;
        if (isspace(c)) break;
        if (c < '0') return 1;
        c -= '0'; if (c > 9) return 1;
        n *= 10; n += c;
        if (n > UINT_MAX) return 1;
    }
    *result = n;
    return 0;
}

int _bc_canvas_load_parse_comment(FILE* file) {
    int c = fgetc(file); if (c < 0) return 1;
    if (c != '#') return ungetc(c, file) != c;
    do { if ((c = fgetc(file)) < 0) return 1; } while (c != '\n');
    return 0;
}

int _bc_canvas_load_parse_header(FILE* file) {
    if (fgetc(file) != 'P') return 1;
    if (fgetc(file) != '6') return 1;
    if (!isspace(fgetc(file))) return 1;
    return 0;
}

void bc_canvas_fragment_map(const struct bc_canvas_pixmap* base, const struct bc_canvas_pixmap* frag, unsigned int frag_width, unsigned int frag_height, struct bc_canvas_graymap* result) {
    assert(frag_width <= BC_KIOSK_WIDTH); assert(frag_height <= BC_KIOSK_HEIGHT);
    const unsigned int scan_last_row = BC_KIOSK_HEIGHT - frag_height;
    const unsigned int scan_last_col = BC_KIOSK_WIDTH - frag_width;
    for (unsigned int base_row = 0; base_row < BC_KIOSK_HEIGHT; ++base_row) for (unsigned int base_col = 0; base_col < BC_KIOSK_WIDTH; ++base_col) {
        if (base_row > scan_last_row || base_col > scan_last_col) { result->gray[base_row][base_col] = UCHAR_MAX; continue; }
        unsigned long long int frag_squared_distance_sum = 0;
        for (unsigned int frag_row = 0; frag_row < frag_height; ++frag_row) for (unsigned int frag_col = 0; frag_col < frag_width; ++frag_col) {
            const struct bc_canvas_rgb* base_rgb = &base->rgb[base_row + frag_row][base_col + frag_col];
            const struct bc_canvas_rgb* frag_rgb = &frag->rgb[frag_row][frag_col];
            int dr = base_rgb->r - frag_rgb->r;
            int dg = base_rgb->g - frag_rgb->g;
            int db = base_rgb->b - frag_rgb->b;
            frag_squared_distance_sum += dr*dr + dg*dg + db*db;
            if (frag_squared_distance_sum > UCHAR_MAX) { frag_squared_distance_sum = UCHAR_MAX ; break; }
        }
        result->gray[base_row][base_col] = frag_squared_distance_sum;
    }
}

void bc_canvas_scan_less_than(const struct bc_canvas_graymap* canvas, unsigned int threshold, int* row, int* col) {
    *col += 1;
    if (*col < 0) *col = 0; else if (*col >= BC_KIOSK_WIDTH) { *col = 0; *row += 1; }
    if (*row < 0) *row = 0;
    for (; *col < BC_KIOSK_WIDTH; ++*col) if (canvas->gray[*row][*col] <= threshold) return;
    for (++*row; *row < BC_KIOSK_HEIGHT; ++*row) for (*col = 0; *col < BC_KIOSK_WIDTH; ++*col) if (canvas->gray[*row][*col] <= threshold) return;
    *col = *row = -1;
}

void bc_canvas_unpack(unsigned char* from, struct bc_canvas_pixmap* to, unsigned int* width, unsigned int* height) {
    *width = from[0]; *width *= 0x100; *width += from[1];
    *height = from[2]; *height *= 0x100; *height += from[3];
    FILE* file = fmemopen(&from[4], 3 * *width * *height, "r");
    for (unsigned int row = 0; row < *height; ++row) for (unsigned int col = 0; col < *width; ++col) {
        struct bc_canvas_rgb* rgb = &to->rgb[row][col];
        rgb->r = fgetc(file);
        rgb->g = fgetc(file);
        rgb->b = fgetc(file);
    }
    fclose(file);
}

