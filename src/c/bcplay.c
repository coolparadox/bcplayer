#include <assert.h>
#include <stdio.h>
#include <sys/stat.h>
#include <time.h>

#include "bcplay_conf.h"
#include "bcplay_kiosk.h"
#include "bcplay_log.h"
#include "bcplay_perception.h"
#include "bcplay_perform.h"
#include "bcplay_screenshot.h"
#include "bcplay_sm.h"

#define BC_MODULE "main"

int main(int argc, char** argv) {

    {
        FILE* pid_file = fopen("/home/bcplayer1/bcplay.pid", "w");
        fprintf(pid_file, "%d\n", getpid());
        fclose(pid_file);
    }

#ifdef BC_SPINLOCK
    {
        volatile int i = 1; while (i);  // Wait for the debugger to attach.
    }
#endif  // BC_SPINLOCK

    // Setup logging.
    assert(BC_PLAYER_USERID == getuid());
    setlogmask(LOG_UPTO(BC_LOG_LEVEL));
    openlog(NULL, LOG_PID, LOG_LOCAL0);
    log_notice("player %u: hello", BC_PLAYER_USERID);

    // Play!
    bc_sm_init();
    bcplay_kiosk_spawn();
    while (bc_sm_get_state() != BC_STATE_END) {
        log_debug("loop");
        struct bc_canvas_pixmap screenshot; if (bc_screenshot_acquire(&screenshot)) fail("cannot acquire screenshot");
        struct bc_perception sight; if (bc_perceive(&screenshot, &sight)) fail("cannot see the gameplay");
        struct bc_sm_recommendation advice; if (bc_sm_assess(&sight, &advice)) fail("cannot assess the gameplay");
        if (bc_perform(&advice.hint)) fail("cannot act on gameplay");
        log_debug("sleep: %u", advice.sleep);
        time_t wakeup_time = time(NULL) + advice.sleep;
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
