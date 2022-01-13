// #define _XOPEN_SOURCE 500
#include <errno.h>
// #include <ftw.h>
#include <string.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

#include "bcplay_log.h"

#include "bcplay_kiosk.h"

extern pid_t bc_kiosk_pid;

// int rmFiles(const char* pathname, const struct stat* sb, int typeflag, struct FTW* ftwbuf);

pid_t bcplay_kiosk_spawn(void) {
    // nftw("/home/bcplayer1/.cache/mozilla/firefox", rmFiles, 8, FTW_DEPTH|FTW_MOUNT|FTW_PHYS);
    switch (bc_kiosk_pid = fork()) {
        case 0:
            // execl("/usr/bin/xterm", "", NULL);
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
    log_debug("child process %u started for the kiosk", bc_kiosk_pid);
}

int bcplay_kiosk_is_alive(int* answer) {
    pid_t pid = waitpid(bc_kiosk_pid, NULL, WNOHANG);
    if (pid < 0) FAIL("cannot check for child pid %u status: %s", bc_kiosk_pid, strerror(errno));
    *answer = pid == 0;
    return 0;
}

// int rmFiles(const char* pathname, const struct stat* sb, int typeflag, struct FTW* ftwbuf) {
//     if (remove(pathname)) FAIL("cannot remove '%s': %s", pathname, strerror(errno));
//     return 0;
// }

