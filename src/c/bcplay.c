#include <assert.h>
#include <stdio.h>

#include "bcplay_conf.h"
#include "bcplay_kiosk.h"
#include "bcplay_log.h"
#include "bcplay_perception.h"
#include "bcplay_perform.h"
#include "bcplay_sm.h"

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
        int is_kiosk_alive; if (bcplay_kiosk_is_alive(&is_kiosk_alive)) FAIL("cannot tell whether kiosk is alive");
        if (!is_kiosk_alive) { log_debug("kiosk termination"); break; }
        struct bc_perception sight; if (bc_perceive(&sight)) FAIL("cannot see the gameplay");
        struct bc_sm_recommendation advice; if (bc_sm_assess(&sight, &advice)) FAIL("cannot assess the gameplay");
        if (bc_perform(&advice.hint)) FAIL("cannot act on gameplay");
        sleep(advice.sleep);
        // TODO: trace the game walkthough
    }
    log_notice("player %u: bye", BC_PLAYER_USERID);
    return 0;

}
