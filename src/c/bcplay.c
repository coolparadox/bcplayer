#include <assert.h>
#include <errno.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include "bcplay_conf.h"
#include "bcplay_log.h"
#include "bcplay_perception.h"
#include "bcplay_random.h"

// FIXME: bcplay_sm.h
// Player state machine
enum states {
    STATE_START,  // Wait for the kiosk window.
    STATE_END,  // The kiosk process is over.
};

struct context {
    time_t now;
    pid_t kiosk_pid;
    enum states state;
    struct bc_perception sight;
    unsigned int sleep;
};

// FIXME: bcplay_interact.h?
int assess(struct context* ctx);

int main(int argc, char** argv) {

    {
        FILE* pid_file = fopen("/home/bcplayer1/bcplay.pid", "w");
        fprintf(pid_file, "%d\n", getpid());
        fclose(pid_file);
    }

#ifdef BC_DEBUG
    {
        volatile int i = 1; while (i);  // Wait for the debugger to attach.
    }
#endif  // BC_DEBUG

    // Setup logging.
    assert(BC_PLAYER_USERID == getuid());
    setlogmask(LOG_UPTO(BC_LOG_LEVEL));
    openlog(NULL, LOG_PID, LOG_LOCAL0);
    log_notice("player %u: started", BC_PLAYER_USERID);

    // Initialize the player state.
    struct context ctx;
    time(&ctx.now);
    ctx.state = STATE_START;
    ctx.sleep = 0;

    // Spawn a kiosk window.
    switch (ctx.kiosk_pid = fork()) {
        case 0:
            //execl("/usr/bin/xterm", "", NULL);
            execl("/usr/bin/firefox"
                    , "--new-instance"
                    , "--first-startup"
                    , "--no-remote"
                    , "--private-window"
                    , "--kiosk"
                    , NULL);
            FAIL("cannot exec kiosk: %s", strerror(errno));
        case -1:
            FAIL("cannot fork kiosk: %s", strerror(errno))
    }
    log_debug("kiosk pid %u started at display %s", ctx.kiosk_pid, BC_KIOSK_DISPLAY);

    // Play!
    while (ctx.state != STATE_END) {
        sleep(ctx.sleep);
        if (assess(&ctx)) FAIL("cannot understand the game");
        // TODO: generate trace records for the context.
    }
    log_notice("player %u: terminated", BC_PLAYER_USERID);
    return 0;

}

int assess(struct context* ctx) {

    // Check if the kiosk is still alive.
    int kiosk_pid_status;
    pid_t pid = waitpid(ctx->kiosk_pid, &kiosk_pid_status, WNOHANG);
    switch (pid) {
        case -1: FAIL("cannot check for child pid %u status: %s", ctx->kiosk_pid, strerror(errno));
        case 0: break;
        default:
            log_debug("kiosk termination");
            ctx->state = STATE_END;
            return 0;
    }

    // Interact with the game screen.
    ctx->sleep = bc_random_sample_uniform(5, 15);
    if (bc_perceive(&ctx->sight)) FAIL("cannot perceive screen");
    switch (ctx->state) {

        case STATE_START:

            // Keep waiting for the kiosk window to appear.
            if (ctx->sight.glimpse != BC_GLIMPSE_KIOSK) return 0;
            // The kiosk window has just appeared.
            // TODO: now what?
            ctx->state = STATE_END;
            return 0;

        default: PANIC("unknown state %u", ctx->state);

    }
}

