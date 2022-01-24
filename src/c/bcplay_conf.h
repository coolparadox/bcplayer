#ifndef BCPLAY_CONF_H_SEEN
#define BCPLAY_CONF_H_SEEN

#include <syslog.h>

#define BC_LOG_LEVEL LOG_DEBUG
#define BC_PLAYER_USERID 1002
#define BC_KIOSK_WIDTH 960
#define BC_KIOSK_HEIGHT 600
#define BC_KIOSK_DISPLAY ":1"
#define BC_PATH_MAX 0x200
#define BC_TRACE_DIR "/home/bcplayer1/bctrace"
#define BC_PLANNING_HINTS_SIZE 0x100
#define BC_PERFORM_EVENT_DELAY_US 100000
#define BC_PERFORM_MOUSE_CLICK_DELAY_US 500000
#define BC_CHARACTER_FASTEST_RECOVERY_TIME_MIN 200
#define BC_AMOUNT_OF_CHARACTERS 5

#endif  // BCPLAY_CONF_H_SEEN
