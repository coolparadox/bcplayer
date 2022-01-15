// #define _XOPEN_SOURCE 500
// #include <ftw.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

#include "bcplay_log.h"

#include "bcplay_kiosk.h"

#define BC_MODULE "screenshot"

extern pid_t _bc_kiosk_pid;

// int rmFiles(const char* pathname, const struct stat* sb, int typeflag, struct FTW* ftwbuf);

pid_t bcplay_kiosk_spawn(void) {
    // nftw("/home/bcplayer1/.cache/mozilla/firefox", rmFiles, 8, FTW_DEPTH|FTW_MOUNT|FTW_PHYS);
    switch (_bc_kiosk_pid = fork()) {
        case 0:
            // execl("/usr/bin/xterm", "", NULL);
            execl("/usr/bin/firefox"
                    , "--new-instance"
                    , "--first-startup"
                    , "--no-remote"
                    , "--private-window"
                    , "--kiosk"
                    , NULL);
            fail("cannot exec kiosk: %m");
        case -1:
            fail("cannot fork kiosk: %m")
    }
    log_debug("kiosk pid: %u", _bc_kiosk_pid);
}

int bcplay_kiosk_is_alive(int* answer) {
    pid_t pid = waitpid(_bc_kiosk_pid, NULL, WNOHANG);
    if (pid < 0) fail("cannot check for child pid %u status: %m", _bc_kiosk_pid);
    *answer = pid == 0;
    return 0;
}

// int rmFiles(const char* pathname, const struct stat* sb, int typeflag, struct FTW* ftwbuf) {
//     if (remove(pathname)) fail("cannot remove '%s': %m", pathname);
//     return 0;
// }

