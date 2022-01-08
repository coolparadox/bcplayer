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

// BC state machine
enum states {
    STATE_START,
    STATE_END,
};

void sleep_random(unsigned int min, unsigned int max);
void assess(void);

pid_t kiosk_pid;
enum states state;

int main(int argc, char** argv) {

    {
        FILE* pid_file = fopen("/home/bcplayer1/bcplay.pid", "w");
        fprintf(pid_file, "%d\n", getpid());
        fclose(pid_file);
    }

#ifdef BC_DEBUG
    volatile int i = 1; while (i);
#endif  // BC_DEBUG

    // Setup logging
    assert(PLAYER_USERID == getuid());
    setlogmask(LOG_UPTO(LOG_LEVEL));
    openlog(NULL, LOG_PID, LOG_LOCAL0);
    log_notice("player %u: started", PLAYER_USERID);

    // Spawn kiosk
    switch (kiosk_pid = fork()) {
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
    log_debug("kiosk pid %u started at display %s", kiosk_pid, KIOSK_DISPLAY);

    // Play
    state = STATE_START;
    while (state != STATE_END) {
        sleep_random(5, 15);
        assess();
    }
    log_notice("player %u: terminated", PLAYER_USERID);
    return 0;

}

void sleep_random(unsigned int min, unsigned int max) {
    assert(max >= min);
    srand(time(0));
    unsigned long long int t = rand();
    t *= max - min;
    t += (unsigned long long int) RAND_MAX * min;
    t /= RAND_MAX;
    sleep(t);
}

enum glimpses {
    GLIMPSE_UNKNOWN,
    GLIMPSE_KIOSK,  // browser first appeared
};

enum glimpses glimpse(void);

void assess(void) {
    int kiosk_pid_status;
    pid_t pid = waitpid(kiosk_pid, &kiosk_pid_status, WNOHANG);
    switch (pid) {
        case -1:
            FAIL("cannot check for child pid %u status: %s", kiosk_pid, strerror(errno));
        case 0:
            break;
        default:
            log_debug("kiosk termination");
            state = STATE_END;
            return;
    }
    switch (state) {
        case STATE_START:
            if (glimpse() != GLIMPSE_KIOSK) return;
            state = STATE_END;
            return;
        default:
            PANIC("unknown state %u", state);
    }
}

enum glimpses glimpse(void) {
    return GLIMPSE_UNKNOWN;
}

