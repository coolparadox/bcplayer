#include <assert.h>
#include <errno.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <syslog.h>
#include <unistd.h>

#define LOG_LEVEL LOG_DEBUG
#define PLAYER_USERID 1002
#define KIOSK_WIDTH 960
#define KIOSK_HEIGHT 600
#define KIOSK_DISPLAY ":1"

#define XSTR(X) STR(X)
#define STR(X) #X
#define XSTARTUP_PATH "/tmp/" XSTR(PLAYER_USERID) ".xstartup"

#define log(LEVEL, ...) syslog(LOG_MAKEPRI(LOG_USER, LEVEL), __VA_ARGS__)

#define log_debug(...) log(LOG_DEBUG, "debug: " __VA_ARGS__)
#define log_notice(...) log(LOG_NOTICE, "notice: " __VA_ARGS__)
#define log_err(...) log(LOG_ERR, "error: " __VA_ARGS__)

#define FAIL(...) { \
    log_err(__VA_ARGS__); \
    exit(EXIT_FAILURE); \
}

int main(int argc, char** argv) {

    // Setup logging
    assert(PLAYER_USERID == getuid());
    char* username; {
        struct passwd* user = getpwuid(PLAYER_USERID); assert(user);
        username = strndup(user->pw_name, 0x10);
    }
    setlogmask(LOG_UPTO(LOG_LEVEL));
    openlog(username, LOG_PID, LOG_LOCAL0);
    log_notice("player %u: started", PLAYER_USERID);

    // Spawn kiosk
    pid_t kiosk_pid = fork();
    switch (kiosk_pid) {
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

    // Wait for kiosk termination
    int status;
    if (waitpid(kiosk_pid, &status, 0) == -1) FAIL("cannot wait for kiosk: %s", strerror(errno));
    if (WIFEXITED(status)) log_debug("kiosk return code: %d", WEXITSTATUS(status));

    // Bye
    log_notice("player %u: terminated", PLAYER_USERID);
    return 0;

}
