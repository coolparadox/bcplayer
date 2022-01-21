#include <unistd.h>

#include <X11/Xlib.h>

#include "bcplay_log.h"

#include "bcplay_perform.h"

#define BC_MODULE "perform"

int _bc_perform_keyboard_click(Display* display, const struct bc_hint_keyboark_click* detail) {

    int dummy;
    XEvent event = { 0 };
    event.xkey.display = display;
    event.xkey.root = XDefaultRootWindow(display);
    XGetInputFocus(display, &event.xkey.window, &dummy);
    event.xkey.subwindow = None;
    event.xkey.time = CurrentTime;
    event.xkey.x = event.xkey.y = 1;
    event.xkey.x_root = event.xkey.y_root = 1;
    event.xkey.same_screen = True;
    if (!(event.xkey.keycode = XKeysymToKeycode(display, detail->symbol))) fail("unknown keysym: %u", detail->symbol);
    //event.xkey.state = detail->modifier;
    if (!(event.xkey.state = XKeysymToKeycode(display, detail->modifier))) fail("unknown keysym: %u", detail->modifier);

    event.xkey.type = KeyPress;
    if (!XSendEvent(display, InputFocus, True, KeyPressMask, &event)) fail("cannot send keypress event");

    event.xkey.type = KeyRelease;
    if (!XSendEvent(display, InputFocus, True, KeyReleaseMask, &event)) fail("cannot send keyrelease event");

    return 0;
}

int bc_perform(const struct bc_planning_hint* hints) {
    Display* display = NULL;
#define cleanup_fail(...) { if (display) XCloseDisplay(display); fail(__VA_ARGS__); }
    display = XOpenDisplay(NULL); if (!display) cleanup_fail("cannot open display");
     do {
        switch (hints->type) {
            case 0: break;
            case BC_HINT_KEYBOARD_CLICK: if(_bc_perform_keyboard_click(display, &hints->detail.keyboard_click)) cleanup_fail("cannot perform keyboard click"); break;
            default: panic("unknown hint type %i", hints->type);
        }
        log_warning("TODO: implement human pauses between events");
        usleep(10000);
    } while (hints++->type);
    XCloseDisplay(display);
}

