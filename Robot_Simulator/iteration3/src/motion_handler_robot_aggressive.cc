/**
 * @file motion_handler_robot_aggressive.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 *
 */
#include "src/motion_handler_robot_aggressive.h"


NAMESPACE_BEGIN(csci3081);


Pose MotionHandlerRobotAggressive::UpdateSpeed(double lsr_L, double lsr_R) {
  double new_position_ = 0.001 * ((lsr_R > lsr_L) ? lsr_R : lsr_L);
  return Pose(clamp_vel(5 + new_position_*lsr_R), clamp_vel(5 +
    new_position_*lsr_L));
  }

NAMESPACE_END(csci3081);
