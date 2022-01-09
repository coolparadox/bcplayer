#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "bcplay_log.h"

#include "bcplay_perception.h"

int bc_perceive(struct bc_perception* result) {

    result->glimpse = BC_GLIMPSE_UNKNOWN;

    // FIXME: encapsulate this block in separate call
    Display* display = XOpenDisplay(NULL);
    // FIXME: parameterize log & fail
    if (!display) FAIL("cannot open display");
    Window root = DefaultRootWindow(display);
    XWindowAttributes attr = { 0 };
    XGetWindowAttributes(display, root, &attr);
    if (attr.width != BC_KIOSK_WIDTH) FAIL("unexpected screen width %u", attr.width);
    if (attr.height != BC_KIOSK_HEIGHT) FAIL("unexpected screen height %u", attr.height);
    XImage* img = XGetImage(display, root, 0, 0, BC_KIOSK_WIDTH, BC_KIOSK_HEIGHT, AllPlanes, ZPixmap);
    if (!img) FAIL("cannot get screenshot");
    // FIXME: parameterize pixel walk
    for (unsigned int row = 0; row < BC_KIOSK_HEIGHT; ++row) for (unsigned int col = 0; col < BC_KIOSK_WIDTH; ++col) {
        unsigned long rgb = XGetPixel(img, col, row);
        result->screenshot.b[row][col] = rgb % 0x100; rgb /= 0x100;
        result->screenshot.g[row][col] = rgb % 0x100; rgb /= 0x100;
        result->screenshot.r[row][col] = rgb % 0x100; rgb /= 0x100;
        if (rgb) FAIL("unsupported pixel format");
    }
    XDestroyImage(img);
    XCloseDisplay(display);

    // FIXME: perceive
    return 0;

}

