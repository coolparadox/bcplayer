#ifndef BCPLAY_HINT_TYPES_H_SEEN
#define BCPLAY_HINT_TYPES_H_SEEN

#include <X11/keysym.h>

#include "bcplay_canvas_types.h"

#define BC_HINT_KEYSEQ_SIZE 0x80

enum bc_hint_type {
    BC_HINT_END = 0,
    BC_HINT_KEYBOARD_CLICK,
    BC_HINT_KEYBOARD_SEQUENCE,
    BC_HINT_MOUSE_CLICK,
    BC_HINT_MOUSE_MOVE,
    BC_HINT_MOUSE_DRAG,
};

struct bc_hint_keyboark_click {
    unsigned char key[BC_HINT_KEYSEQ_SIZE];  // Keysym name as null terminated string
};

struct bc_hint_keyboark_sequence {
    unsigned char keys[BC_HINT_KEYSEQ_SIZE];  // Characters to send as null terminated string
};

struct bc_hint_mouse_click {
    struct bc_point coord;  // Location to click with mouse
};

struct bc_hint_mouse_move {
    struct bc_point coord;  // Location to rest mouse pointer
};

struct bc_hint_mouse_drag {
    struct bc_point from;
    struct bc_point to;
};

struct bc_planning_hint {
    enum bc_hint_type type;
    union {
        struct bc_hint_keyboark_click keyboard_click;
        struct bc_hint_keyboark_sequence keyboard_sequence;
        struct bc_hint_mouse_click mouse_click;
        struct bc_hint_mouse_move mouse_move;
        struct bc_hint_mouse_drag mouse_drag;
    } detail;
};

#endif  // BCPLAY_HINT_TYPES_H_SEEN
