#ifndef BCPLAY_PLANNING_H_SEEN
#define BCPLAY_PLANNING_H_SEEN

#include "bcplay_perception_types.h"
#include "bcplay_planning_types.h"

void bc_planning_init();

enum bc_planning_states bc_planning_get_state();

int bc_planning_assess(const struct bc_perception* sight, struct bc_planning_recommendation* advice);

#endif  // BCPLAY_PLANNING_H_SEEN
