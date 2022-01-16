#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "bcplay_canvas.h"
#include "bcplay_log.h"

#include "bcplay_screenshot.h"

#define BC_MODULE "screenshot"

#ifdef close_fail
#undef close_fail
#endif
#define close_fail(...) { if (img) XDestroyImage(img); if (display) XCloseDisplay(display); fail(__VA_ARGS__); }
int bc_screenshot_acquire(struct bc_canvas* to) {
    Display* display = NULL;
    XImage* img = NULL;
    display = XOpenDisplay(NULL); if (!display) close_fail("cannot open display");
    Window root = DefaultRootWindow(display);
    XWindowAttributes attr; XGetWindowAttributes(display, root, &attr);
    if (attr.width != BC_KIOSK_WIDTH) close_fail("unexpected screen width %u", attr.width);
    if (attr.height != BC_KIOSK_HEIGHT) close_fail("unexpected screen height %u", attr.height);
    img = XGetImage(display, root, 0, 0, BC_KIOSK_WIDTH, BC_KIOSK_HEIGHT, AllPlanes, ZPixmap); if (!img) close_fail("cannot get screenshot");
    for (unsigned int row = 0; row < BC_KIOSK_HEIGHT; ++row) for (unsigned int col = 0; col < BC_KIOSK_WIDTH; ++col) {
        unsigned long rgb = XGetPixel(img, col, row);
        to->b[row][col] = rgb % 0x100; rgb /= 0x100;
        to->g[row][col] = rgb % 0x100; rgb /= 0x100;
        to->r[row][col] = rgb % 0x100; rgb /= 0x100;
        if (rgb) close_fail("unsupported pixel format");
    }
    XDestroyImage(img);
    XCloseDisplay(display);
    log_debug("acquired: %s", bc_canvas_make_dump(to));
    return 0;
}
