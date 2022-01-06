#include <errno.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <syslog.h>
#include <unistd.h>

#define LOG_FACILITY LOG_LOCAL0
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
    setlogmask(LOG_UPTO(LOG_LEVEL));
    openlog(NULL, LOG_PERROR|LOG_PID, LOG_FACILITY);
    log_notice("player %u: starting", PLAYER_USERID);
    if (getuid() != PLAYER_USERID) FAIL("my real user id (%u) is not %u", getuid(), PLAYER_USERID);

    log_debug("xstartup_path: %s", XSTARTUP_PATH);
    FILE* xstartup_file = fopen(XSTARTUP_PATH, "w");
    fprintf(xstartup_file, 
        "#!/usr/bin/env bash\n"
        "unset SESSION_MANAGER\n"
        "unset DBUS_SESSION_BUS_ADDRESS\n"
        "i3 &\n"
        "exec xterm\n"
        "exec firefox --new-instance --first-startup --sync --no-remote --private-window='http://app.bombcrypto.io/'\n");
    fclose(xstartup_file);
    if (chmod(XSTARTUP_PATH, S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IWGRP|S_IXGRP|S_IROTH)) FAIL("cannot change permissions of '%s': %s", XSTARTUP_PATH, strerror(errno));

    // Spawn vnc server
    pid_t vncserver_pid = fork();
    switch (vncserver_pid) {
        case 0:
            execl("/usr/bin/vncserver", KIOSK_DISPLAY, "-geometry", XSTR(KIOSK_WIDTH) "x" XSTR(KIOSK_HEIGHT), "-autokill", "-xstartup", XSTARTUP_PATH, "-SecurityTypes", "None", NULL);
            FAIL("cannot exec vnc server: %s", strerror(errno));
        case -1:
            FAIL("cannot fork vnc server: %s", strerror(errno))
    }
    log_debug("vnc server pid %u started at display %s", vncserver_pid, KIOSK_DISPLAY);
    int status;
    if (waitpid(vncserver_pid, &status, 0) == -1) FAIL("cannot wait for vnc server: %s", strerror(errno));
    if (WIFEXITED(status)) log_debug("vnc server return code: %d", WEXITSTATUS(status));

    // Spawn vnc viewer
    pid_t vncviewer_pid = fork();
    switch (vncviewer_pid) {
        case 0:
            execl("/usr/bin/vncviewer", KIOSK_DISPLAY, NULL);
            FAIL("cannot exec vnc viewer: %s", strerror(errno));
        case -1:
            FAIL("cannot fork vnc viewer: %s", strerror(errno))
    }
    log_debug("vnc viewer pid %u started at display %s", vncviewer_pid, KIOSK_DISPLAY);

    if (waitpid(vncviewer_pid, &status, 0) == -1) FAIL("cannot wait for vnc viewer: %s", strerror(errno));
    if (WIFEXITED(status)) log_debug("vnc viewer return code: %d", WEXITSTATUS(status));

    log_notice("player %u: terminated", PLAYER_USERID);
    return 0;
}
