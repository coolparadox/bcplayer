#ifndef BCPLAY_HINT_TYPES_H_SEEN
#define BCPLAY_HINT_TYPES_H_SEEN

struct bc_point {
    unsigned int x;  // column
    unsigned int y;  // row
};

struct bc_bbox {
    struct bc_point tl;  // top left
    struct bc_point br;  // bottom right
};

enum bc_sm_hints {
    BC_HINT_UNKNOWN,
    BC_HINT_NOTHING,
    BC_HINT_CLICK,
    BC_HINT_DRAG,
    BC_HINT_WRITE,
    BC_HINT_CLICK_WRITE,
    BC_HINT_CHARACTER,
};

struct bc_sm_hint_click {
    struct bc_bbox target;
};

struct bc_sm_hint_drag {
    struct bc_bbox from;
    struct bc_bbox to;
};

struct bc_sm_hint_write {
    const char* text;
    int newline;
};

struct bc_sm_hint_click_write {
    struct bc_sm_hint_click click;
    struct bc_sm_hint_write write;
};

enum bc_character_modifier {
    BC_MODIFIER_CTRL,
    BC_MODIFIER_ALT,
};

struct bc_sm_character {
    char code;
    enum bc_character_modifier modifier;
};

struct bc_sm_hint {
    enum bc_sm_hints hint;
    union {
        struct bc_sm_hint_click click;
        struct bc_sm_hint_drag drag;
        struct bc_sm_hint_write write;
        struct bc_sm_hint_click_write click_write;
        struct bc_sm_character character;
    } detail;
};

#endif  // BCPLAY_HINT_TYPES_H_SEEN
