#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "bcplay_canvas.h"
#include "bcplay_log.h"

#include "bcplay_screenshot.h"

#define BC_MODULE "screenshot"

int bc_screenshot_acquire(struct bc_canvas_pixmap* to) {
    Display* display = NULL;
    XImage* img = NULL;
#define cleanup_fail(...) { if (img) XDestroyImage(img); if (display) XCloseDisplay(display); fail(__VA_ARGS__); }
    display = XOpenDisplay(NULL); if (!display) cleanup_fail("cannot open display");
    Window root = DefaultRootWindow(display);
    XWindowAttributes attr; XGetWindowAttributes(display, root, &attr);
    if (attr.width != BC_KIOSK_WIDTH) cleanup_fail("unexpected screen width %u", attr.width);
    if (attr.height != BC_KIOSK_HEIGHT) cleanup_fail("unexpected screen height %u", attr.height);
    img = XGetImage(display, root, 0, 0, BC_KIOSK_WIDTH, BC_KIOSK_HEIGHT, AllPlanes, ZPixmap); if (!img) cleanup_fail("cannot get screenshot");
    for (unsigned int row = 0; row < BC_KIOSK_HEIGHT; ++row) for (unsigned int col = 0; col < BC_KIOSK_WIDTH; ++col) {
        unsigned long triplet = XGetPixel(img, col, row);
        struct bc_canvas_rgb* rgb = &to->rgb[row][col];
        rgb->b = triplet % 0x100; triplet /= 0x100;
        rgb->g = triplet % 0x100; triplet /= 0x100;
        rgb->r = triplet % 0x100; triplet /= 0x100;
        if (triplet) cleanup_fail("unsupported pixel format");
    }
    XDestroyImage(img);
    XCloseDisplay(display);
    log_debug("acquired: %s", bc_canvas_make_dump(to));
    return 0;
}
