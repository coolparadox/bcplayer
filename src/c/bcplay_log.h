#ifndef BCPLAY_LOG_H_SEEN
#define BCPLAY_LOG_H_SEEN

#include <syslog.h>

#include "bcplay_conf.h"

#ifdef DEBUG
#define BC_DEBUG 1
#else
#define BC_DEBUG 0
#endif  // DEBUG

#define XSTR(X) STR(X)
#define STR(X) #X

#define log(LEVEL, ...) syslog(LOG_MAKEPRI(LOG_USER, LEVEL), XSTR(BC_PLAYER_USERID) ": " __VA_ARGS__)

#define log_debug(...) do { if (BC_DEBUG) log(LOG_DEBUG, "debug: " __VA_ARGS__); } while (0)
#define log_notice(...) log(LOG_NOTICE, "notice: " __VA_ARGS__)
#define log_warning(...) log(LOG_WARNING, "warning: " __VA_ARGS__)
#define log_err(...) log(LOG_ERR, "error: " __VA_ARGS__)
#define log_panic(...) log(LOG_CRIT, "panic: " __VA_ARGS__)

#define FAIL(...) { \
    log_err(__VA_ARGS__); \
    return 1; \
}

#define PANIC(...) { \
    log_panic(__VA_ARGS__); \
    return 1; \
}

#endif  // BCPLAY_LOG_H_SEEN
