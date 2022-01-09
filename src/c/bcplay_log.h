#ifndef BCPLAY_LOG_H_SEEN
#define BCPLAY_LOG_H_SEEN

#include <syslog.h>

#define XSTR(X) STR(X)
#define STR(X) #X

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

#endif  // BCPLAY_LOG_H_SEEN
