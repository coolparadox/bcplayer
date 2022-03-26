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

#ifndef BCPLAY_CONF_H_SEEN
#define BCPLAY_CONF_H_SEEN

#include <syslog.h>

#define BC_LOG_LEVEL LOG_DEBUG
#define BC_PLAYER_USERID 1002
#define BC_KIOSK_WIDTH 960
#define BC_KIOSK_HEIGHT 600
#define BC_KIOSK_DISPLAY ":1"
#define BC_PATH_MAX 0x200
#define BC_TRACE_DIR "/home/bcplayer1/bctrace"
#define BC_PLANNING_HINTS_SIZE 0x100
#define BC_PERFORM_EVENT_DELAY_US 100000
#define BC_PERFORM_MOUSE_CLICK_DELAY_US 500000
#define BC_MOUSE_DRAG_STEPS 30

#define BC_FULL_ENERGY_WAIT_MINIMUM 60
#define BC_FULL_ENERGY_WAIT_START 360
#define BC_FULL_ENERGY_WAIT_ADJUST 1

#endif  // BCPLAY_CONF_H_SEEN
