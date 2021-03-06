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

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#include "bcplay_conf.h"
#include "bcplay_kiosk.h"
#include "bcplay_log.h"
#include "bcplay_perception.h"
#include "bcplay_perform.h"
#include "bcplay_screenshot.h"
#include "bcplay_planning.h"

#define BC_MODULE "main"

extern char _bc_metamask_uuid[];

/*
 * Wish list
 * - watch the gameplay remotely
 * - support a wait perform action (eg, after enabling a character)
 * - know beforehand how much to wait until the next full bar
 * - perceive the energy level of each character
 * - perceive chests and their energy
 * - specialize the character selection
 */

int main(int argc, char** argv) {

    {
        FILE* pid_file = fopen("/home/bcplayer1/bcplay.pid", "w");
        fprintf(pid_file, "%d\n", getpid());
        fclose(pid_file);
    }

    // Setup logging.
    assert(BC_PLAYER_USERID == getuid());
    setlogmask(LOG_UPTO(BC_LOG_LEVEL));
    openlog(NULL, LOG_PID, LOG_LOCAL0);
    log_notice("player %u: hello", BC_PLAYER_USERID);

    // Parse arguments.
    if (argc != 2) fail("missing argument: metamask extension uuid");
    strcpy(_bc_metamask_uuid, argv[1]);
    log_debug("metamask uuid: %s", _bc_metamask_uuid);

    // Play!
    bc_planning_init();
    bcplay_kiosk_spawn();
#ifdef BC_SPINLOCK
    { volatile int i = 1; while (i); }  // Wait for the debugger to attach.
#endif  // BC_SPINLOCK
    while (bc_planning_get_state() != BC_STATE_END) {
        log_debug("loop");
        struct bc_planning_recommendation advice;
        FILE* pause_file = fopen("/home/bcplayer1/bcplay.pause", "r");
        if (pause_file) {
            fclose(pause_file);
            log_info("pause file detected: bypassing actions");
            advice.sleep = 60;
            goto pause_bypass;
        }
        struct bc_canvas_pixmap screenshot; if (bc_screenshot_acquire(&screenshot)) fail("cannot acquire screenshot");
        struct bc_perception sight; if (bc_perceive(&screenshot, &sight)) fail("cannot see the gameplay");
        if (bc_planning_assess(&sight, &advice)) fail("cannot assess the gameplay");
        if (bc_perform(advice.hints)) fail("cannot act on gameplay");
pause_bypass:
        time_t wakeup_time = time(NULL) + advice.sleep;
        log_debug("sleeping for %02u:%02u until %s", advice.sleep / 60, advice.sleep % 60, ctime(&wakeup_time));
        do {
            sleep(1);
            int is_kiosk_alive; if (bcplay_kiosk_is_alive(&is_kiosk_alive)) fail("cannot tell whether kiosk is alive");
            if (!is_kiosk_alive) { log_debug("kiosk termination"); goto end; }
        } while (time(NULL) <= wakeup_time);
    }
end:
    log_notice("player %u: bye", BC_PLAYER_USERID);
    return 0;

}
