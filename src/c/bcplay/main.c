#include <pwd.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>

#define LOG_FACILITY LOG_LOCAL0
#define LOG_LEVEL LOG_DEBUG
#define PLAYER_USERID 1002

#define log(LEVEL, ...) syslog(LOG_MAKEPRI(LOG_USER, LEVEL), __VA_ARGS__)

#define log_debug(...) log(LOG_DEBUG, "debug: " __VA_ARGS__)
#define log_notice(...) log(LOG_NOTICE, "notice: " __VA_ARGS__)
#define log_err(...) log(LOG_ERR, "error: " __VA_ARGS__)

#define FAIL(...) \
{ \
    log_err(__VA_ARGS__); \
    exit(EXIT_FAILURE); \
}

void main(int argc, char** argv)
{
    setlogmask(LOG_UPTO(LOG_LEVEL));
    openlog(NULL, LOG_PERROR|LOG_PID, LOG_FACILITY);
    log_notice("starting for player id %u.", PLAYER_USERID);
    if (getuid() != PLAYER_USERID) FAIL("my real user id (%u) is not %u.", getuid(), PLAYER_USERID);
    FAIL("not yet implemented.")
}
