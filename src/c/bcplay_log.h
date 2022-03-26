/*
 * Copyright 2022 Rafael Lorandi <coolparadox@gmail.com>
 *
 * This file is part of bcplay.
 *
 * bcplay is free software: you can redistribute it and/or modify it under the terms of the GNU
 * General Public License as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * bcplay is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with bcplay. If not, see
 * <https://www.gnu.org/licenses/>.
 */

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

#define log_debug(...) do { if (BC_DEBUG) log(LOG_DEBUG, "debug: " BC_MODULE ": " __VA_ARGS__); } while (0)
#define log_notice(...) log(LOG_NOTICE, "notice: " BC_MODULE ": " __VA_ARGS__)
#define log_info(...) log(LOG_INFO, "notice: " BC_MODULE ": " __VA_ARGS__)
#define log_warning(...) log(LOG_WARNING, "warning: " BC_MODULE ": " __VA_ARGS__)
#define log_err(...) log(LOG_ERR, "error: " BC_MODULE ": " __VA_ARGS__)
#define log_panic(...) log(LOG_CRIT, "panic: " BC_MODULE ": " __VA_ARGS__)

#define fail(...) { \
    log_err(__VA_ARGS__); \
    return 1; \
}

#define panic(...) { \
    log_panic(__VA_ARGS__); \
    return 1; \
}

#endif  // BCPLAY_LOG_H_SEEN
