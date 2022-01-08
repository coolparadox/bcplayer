#include <assert.h>
#include <errno.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <syslog.h>
#include <time.h>
#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#define LOG_LEVEL LOG_DEBUG
#define PLAYER_USERID 1002
#define KIOSK_WIDTH 960
#define KIOSK_HEIGHT 600
#define KIOSK_DISPLAY ":1"

#define XSTR(X) STR(X)
#define STR(X) #X
#define XSTARTUP_PATH "/tmp/" XSTR(PLAYER_USERID) ".xstartup"

#define log(LEVEL, ...) syslog(LOG_MAKEPRI(LOG_USER, LEVEL), XSTR(PLAYER_USERID) ": " __VA_ARGS__)

#define log_debug(...) log(LOG_DEBUG, "debug: " __VA_ARGS__)
#define log_notice(...) log(LOG_NOTICE, "notice: " __VA_ARGS__)
#define log_err(...) log(LOG_ERR, "error: " __VA_ARGS__)
#define log_panic(...) log(LOG_CRIT, "panic: " __VA_ARGS__)

#define FAIL(...) { \
    log_err(__VA_ARGS__); \
    exit(EXIT_FAILURE); \
}

#define PANIC(...) { \
    log_panic(__VA_ARGS__); \
    exit(EXIT_FAILURE); \
}

// Player state machine
enum states {
    STATE_START,  // Wait for the kiosk window.
    STATE_END,  // The kiosk process is over.
};

// What is the game display showing?
enum glimpses {
    GLIMPSE_UNKNOWN,
    GLIMPSE_KIOSK,  // The kiosk clean window.
};

struct context {
    time_t now;
    pid_t kiosk_pid;
    enum states state;
    enum glimpses glimpse;
    XImage* screenshot;
    unsigned int sleep;
};

void assess(struct context* ctx);

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
    assert(PLAYER_USERID == getuid());
    setlogmask(LOG_UPTO(LOG_LEVEL));
    openlog(NULL, LOG_PID, LOG_LOCAL0);
    log_notice("player %u: started", PLAYER_USERID);

    // Initialize the player state.
    struct context ctx;
    time(&ctx.now);
    ctx.state = STATE_START;
    ctx.screenshot = NULL;
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
    log_debug("kiosk pid %u started at display %s", ctx.kiosk_pid, KIOSK_DISPLAY);

    // Play!
    while (ctx.state != STATE_END) {
        sleep(ctx.sleep);
        assess(&ctx);
        // TODO: generate trace records for the context.
    }
    log_notice("player %u: terminated", PLAYER_USERID);
    return 0;

}

void glimpse(struct context* ctx);
unsigned int sample_uniform(unsigned int min, unsigned int max);

void assess(struct context* ctx) {

    // Check if the kiosk is still alive.
    int kiosk_pid_status;
    pid_t pid = waitpid(ctx->kiosk_pid, &kiosk_pid_status, WNOHANG);
    switch (pid) {
        case -1: FAIL("cannot check for child pid %u status: %s", ctx->kiosk_pid, strerror(errno));
        case 0: break;
        default:
            log_debug("kiosk termination");
            ctx->state = STATE_END;
            return;
    }

    // Interact with the game screen.
    ctx->sleep = sample_uniform(5, 15);
    glimpse(ctx);
    switch (ctx->state) {

        case STATE_START:

            // Keep waiting for the kiosk window to appear.
            if (ctx->glimpse != GLIMPSE_KIOSK) return;
            // The kiosk window has just appeared.
            // TODO: now what?
            ctx->state = STATE_END;
            return;

        default: PANIC("unknown state %u", ctx->state);

    }
}

unsigned int sample_uniform(unsigned int min, unsigned int max) {
    assert(max >= min);
    srand(time(0));
    unsigned long long int t = rand();
    t *= max - min;
    t += (unsigned long long int) RAND_MAX * min;
    return t / RAND_MAX;
}

void glimpse(struct context* ctx) {

    ctx->glimpse = GLIMPSE_UNKNOWN;

    Display* display = XOpenDisplay(NULL);  // FIXME: check for errors
    Window root = DefaultRootWindow(display);
    XWindowAttributes attributes = { 0 };
    XGetWindowAttributes(display, root, &attributes);
    if (ctx->screenshot) XDestroyImage(ctx->screenshot);
    ctx->screenshot = XGetImage(display, root, 0, 0, attributes.width, attributes.height, AllPlanes, ZPixmap);  // FIXME: check for errors
    assert(ctx->screenshot);

    log_debug("0x%lX", XGetPixel(ctx->screenshot, 0, 0));

    XCloseDisplay(display);

}
