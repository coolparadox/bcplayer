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

#include <unistd.h>

#include <xdo.h>

#include "bcplay_conf.h"
#include "bcplay_log.h"

#include "bcplay_perform.h"

#define BC_MODULE "perform"

int _bc_perform_keyboard_click(const xdo_t* xdo, const struct bc_hint_keyboark_click* detail) {
    if (xdo_send_keysequence_window(xdo, CURRENTWINDOW, detail->key, BC_PERFORM_EVENT_DELAY_US)) fail("cannot send key click");;
    return 0;
}

int _bc_perform_keyboard_sequence(const xdo_t* xdo, const struct bc_hint_keyboark_sequence* detail) {
    if (xdo_enter_text_window(xdo, CURRENTWINDOW, detail->keys, BC_PERFORM_EVENT_DELAY_US)) fail("cannot send key sequence");;
    return 0;
}

int _bc_perform_mouse_click(const xdo_t* xdo, const struct bc_hint_mouse_click* detail) {
    if (xdo_move_mouse(xdo, detail->coord.col, detail->coord.row, 0)) fail("cannot move mouse");
    usleep(BC_PERFORM_EVENT_DELAY_US);
    if (xdo_mouse_down(xdo, CURRENTWINDOW, 1)) fail("cannot press mouse");
    usleep(BC_PERFORM_MOUSE_CLICK_DELAY_US);
    if (xdo_mouse_up(xdo, CURRENTWINDOW, 1)) fail("cannot release mouse");
    usleep(BC_PERFORM_MOUSE_CLICK_DELAY_US);
    return 0;
}

int _bc_perform_mouse_move(const xdo_t* xdo, const struct bc_hint_mouse_move* detail) {
    if (xdo_move_mouse(xdo, detail->coord.col, detail->coord.row, 0)) fail("cannot move mouse");
    return 0;
}

int _bc_perform_mouse_drag(const xdo_t* xdo, const struct bc_hint_mouse_drag* detail) {
    if (xdo_move_mouse(xdo, detail->from.col, detail->from.row, 0)) fail("cannot move mouse");
    usleep(BC_PERFORM_EVENT_DELAY_US);
    if (xdo_mouse_down(xdo, CURRENTWINDOW, 1)) fail("cannot press mouse");
    usleep(BC_PERFORM_MOUSE_CLICK_DELAY_US);
    int dcol = detail->to.col; dcol -= detail->from.col; dcol /= BC_MOUSE_DRAG_STEPS;
    int drow = detail->to.row; drow -= detail->from.row; drow /= BC_MOUSE_DRAG_STEPS;
    for (unsigned int step = 1; step < BC_MOUSE_DRAG_STEPS; step++) {
        if (xdo_move_mouse_relative(xdo, dcol, drow)) fail("cannot relative move mouse");
        usleep(BC_PERFORM_EVENT_DELAY_US);
    }
    if (xdo_move_mouse(xdo, detail->to.col, detail->to.row, 0)) fail("cannot move mouse");
    usleep(BC_PERFORM_MOUSE_CLICK_DELAY_US);
    if (xdo_mouse_up(xdo, CURRENTWINDOW, 1)) fail("cannot release mouse");
    usleep(BC_PERFORM_MOUSE_CLICK_DELAY_US);
    return 0;
}

int bc_perform(const struct bc_planning_hint* hints) {
    xdo_t* xdo = xdo_new(NULL); if (!xdo) fail("xdo: cannot instantiate");
#define cleanup_fail(...) { xdo_free(xdo); fail(__VA_ARGS__); }
     do {
        switch (hints->type) {
            case 0: break;
            case BC_HINT_KEYBOARD_CLICK:
                if(_bc_perform_keyboard_click(xdo, &hints->detail.keyboard_click)) cleanup_fail("cannot click keyboard");
                break;
            case BC_HINT_KEYBOARD_SEQUENCE:
                if(_bc_perform_keyboard_sequence(xdo, &hints->detail.keyboard_sequence)) cleanup_fail("cannot type keyboard");
                break;
            case BC_HINT_MOUSE_CLICK:
                if(_bc_perform_mouse_click(xdo, &hints->detail.mouse_click)) cleanup_fail("cannot click mouse");
                break;
            case BC_HINT_MOUSE_MOVE:
                if(_bc_perform_mouse_move(xdo, &hints->detail.mouse_move)) cleanup_fail("cannot move mouse");
                break;
            case BC_HINT_MOUSE_DRAG:
                if(_bc_perform_mouse_drag(xdo, &hints->detail.mouse_drag)) cleanup_fail("cannot drag mouse");
                break;
            default: panic("unknown hint type %i", hints->type);
        }
        usleep(BC_PERFORM_EVENT_DELAY_US);
    } while (hints++->type);
    xdo_free(xdo);
}

