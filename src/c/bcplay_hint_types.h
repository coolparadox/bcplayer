#ifndef BCPLAY_HINT_TYPES_H_SEEN
#define BCPLAY_HINT_TYPES_H_SEEN

#include <X11/keysym.h>

#define BC_HINT_KEYSEQ_SIZE 0x40

enum bc_hint_type {
    BC_HINT_END = 0,
    BC_HINT_KEYBOARD_CLICK,
};

struct bc_hint_keyboark_click {
    unsigned char key[BC_HINT_KEYSEQ_SIZE];
};

struct bc_planning_hint {
    enum bc_hint_type type;
    union {
        struct bc_hint_keyboark_click keyboard_click;
    } detail;
};

#endif  // BCPLAY_HINT_TYPES_H_SEEN
