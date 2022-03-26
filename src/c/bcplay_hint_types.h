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
