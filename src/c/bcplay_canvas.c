#include <stdio.h>
#include <time.h>

#include "bcplay_log.h"

#include "bcplay_canvas.h"

#define BC_MODULE "canvas"

extern char _bc_canvas_path[BC_PATH_MAX];

const char* bc_canvas_make_dump(const struct bc_canvas* from) {
    snprintf(_bc_canvas_path, BC_PATH_MAX, "%s/screenshot_%lX.ppm", BC_TRACE_DIR, time(NULL));
    if (bc_canvas_dump(_bc_canvas_path, from)) return "<error>";
    return _bc_canvas_path;
}

#ifdef close_fail
#undef close_fail
#endif
#define close_fail(...) { if (file) fclose(file); fail(__VA_ARGS__); }
int bc_canvas_dump(const char* ppm_path, const struct bc_canvas* from) {
    // http://netpbm.sourceforge.net/doc/ppm.html
    FILE* file = fopen(ppm_path, "w"); if (!file) close_fail("cannot open '%s' for writing: %m", ppm_path);
    if (fprintf(file, "P6 %u %u 255 ", BC_KIOSK_WIDTH, BC_KIOSK_HEIGHT) < 0) close_fail("cannot dump ppm header to '%s'", ppm_path);
    for (unsigned int row = 0; row < BC_KIOSK_HEIGHT; ++row) for (unsigned int col = 0; col < BC_KIOSK_WIDTH; ++col) {
        if (fputc(from->r[row][col], file) < 0) close_fail("cannot dump ppm data to '%s'", ppm_path);
        if (fputc(from->g[row][col], file) < 0) close_fail("cannot dump ppm data to '%s'", ppm_path);
        if (fputc(from->b[row][col], file) < 0) close_fail("cannot dump ppm data to '%s'", ppm_path);
    }
    if (fclose(file)) fail("cannot close '%s': %m", ppm_path);
    return 0;
}

int bc_canvas_restore(const char* ppm_path, struct bc_canvas* to) {
    // http://netpbm.sourceforge.net/doc/ppm.html
    FILE* file = fopen(ppm_path, "r"); if (!file) fail("cannot open '%s' for reading: %m", ppm_path);
    unsigned int width, height;
    if (fscanf(file, "P6 %u %u 255 ", &width, &height) != 2) close_fail("cannot understand '%s' as a binary ppm file", ppm_path);
    if (width != BC_KIOSK_WIDTH || height != BC_KIOSK_HEIGHT) close_fail("screenshot dimensions mismatch in '%s': expected %ux%u, found %ux%u", ppm_path, BC_KIOSK_WIDTH, BC_KIOSK_HEIGHT, width, height);
    for (unsigned int row = 0; row < BC_KIOSK_HEIGHT; ++row) for (unsigned int col = 0; col < BC_KIOSK_WIDTH; ++col) {
        if ((to->r[row][col] = fgetc(file)) >= 0x100) close_fail("cannot read '%s'", ppm_path);
        if ((to->g[row][col] = fgetc(file)) >= 0x100) close_fail("cannot read '%s'", ppm_path);
        if ((to->b[row][col] = fgetc(file)) >= 0x100) close_fail("cannot read '%s'", ppm_path);
    }
    if(fclose(file)) fail("cannot close '%s': %m", ppm_path);
    return 0;
}
