#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "bcplay_log.h"

#include "bcplay_kiosk.h"

// FIXME: externalize
extern pid_t bc_kiosk_pid;

pid_t bcplay_kiosk_spawn(void) {
    switch (bc_kiosk_pid = fork()) {
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
    log_debug("child process %u started for the kiosk", bc_kiosk_pid);
}

int bcplay_kiosk_is_alive(int* answer) {
    pid_t pid = waitpid(bc_kiosk_pid, NULL, WNOHANG);
    if (pid < 0) FAIL("cannot check for child pid %u status: %s", bc_kiosk_pid, strerror(errno));
    *answer = pid == 0;
    return 0;
}

