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

#ifndef BCPLAY_PERCEPTION_TYPES_H_SEEN
#define BCPLAY_PERCEPTION_TYPES_H_SEEN

#include "bcplay_canvas_types.h"
#include "bcplay_conf.h"

enum bc_glimpse {
    BC_GLIMPSE_UNKNOWN,
    BC_GLIMPSE_BLACK,
    BC_GLIMPSE_KIOSK_UPDATED,
    BC_GLIMPSE_KIOSK_CLEAN,
    BC_GLIMPSE_APPSITE_CONNECT_WALLET,
    BC_GLIMPSE_METAMASK_UNLOCK,
    BC_GLIMPSE_METAMASK_UNLOCKED,
    BC_GLIMPSE_METAMASK_SIGNATURE_REQUEST,
    BC_GLIMPSE_GAME_KIOSK_UNSCROLLED,
    BC_GLIMPSE_GAME_KIOSK_SCROLLED,
    BC_GLIMPSE_GAME_SELECTION,
    BC_GLIMPSE_GAME_ONGOING,
    BC_GLIMPSE_GAME_PAUSED,
    BC_GLIMPSE_AUTOMATIC_EXIT,
    BC_GLIMPSE_GAME_CHARACTERS,
    BC_GLIMPSE_ERROR_OTHER,
    BC_GLIMPSE_LOADING,
    BC_GLIMPSE_GAME_TERM_ACCEPTANCE_UNSELECTED,
    BC_GLIMPSE_GAME_TERM_ACCEPTANCE_SELECTED,
    BC_GLIMPSE_GAME_LOGIN_WITH_METAMASK,
};

union bc_perception_detail {
    struct {
        struct bc_bbox not_now;  // "Not now" clickable text
    } kiosk_updated;
    struct {
        struct bc_bbox connect_wallet;  // "Connect wallet" clickable text
    } appsite_connect_wallet;
    struct {
        struct bc_bbox unlock;  // "Unlock" clickable text
    } metamask_unlock;
    struct {
        struct bc_bbox sign;  // "Sign" clickable text
    } metamask_signature_request;
    struct {
        struct bc_bbox fullscreen;  // Fullscreen button
    } game_kiosk_scrolled;
    struct {
        struct bc_bbox treasure_hunt;  // "treasure hunt" clickable text
    } game_selection;
    struct {
        struct bc_bbox exit;  // game exit button
    } game_ongoing;
    struct {
        struct bc_bbox heroes;  // "heroes" clickable text
    } game_paused;
    struct {
        int has_full;  // has a full energy character?
        struct bc_bbox work;  // "work" clickable text if has_full
    } game_characters;
    struct {
        struct bc_bbox checkbox;  // game term acceptance checkbox
    } game_term_acceptance_unselected;
    struct {
        struct bc_bbox button;  // game term acceptance button
    } game_term_acceptance_selected;
    struct {
        struct bc_bbox button;  // metamask oprion clickable area
    } game_login_with_metamask;
};

struct bc_perception {
    enum bc_glimpse glimpse;
    union bc_perception_detail detail;
};

#endif  // BCPLAY_PERCEPTION_TYPES_H_SEEN
