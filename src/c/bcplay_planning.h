#ifndef BCPLAY_PLANNING_H_SEEN
#define BCPLAY_PLANNING_H_SEEN

#include "bcplay_perception_types.h"
#include "bcplay_planning_types.h"

void bc_sm_init();

enum bc_sm_states bc_sm_get_state();

int bc_sm_assess(const struct bc_perception* sight, struct bc_sm_recommendation* advice);

#endif  // BCPLAY_PLANNING_H_SEEN
