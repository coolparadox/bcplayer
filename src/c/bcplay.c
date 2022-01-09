#include <assert.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "bcplay_conf.h"
#include "bcplay_kiosk.h"
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
    log_notice("player %u: hello", BC_PLAYER_USERID);

    // Initialize the player state.
    struct context ctx;
    time(&ctx.now);
    ctx.state = STATE_START;
    ctx.sleep = 0;

    // Spawn a kiosk window.
    bcplay_kiosk_spawn();

    // Play!
    while (ctx.state != STATE_END) {

        sleep(ctx.sleep);

        {
            int is_kiosk_alive;
            if (bcplay_kiosk_is_alive(&is_kiosk_alive)) FAIL("cannot tell whether kiosk is alive");
            if (!is_kiosk_alive) { log_debug("kiosk termination"); ctx.state = STATE_END; break; }
        }

        if (assess(&ctx)) FAIL("cannot understand the game");
        // TODO: generate trace records for the context.

    }

    log_notice("player %u: bye", BC_PLAYER_USERID);
    return 0;

}

int assess(struct context* ctx) {

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

