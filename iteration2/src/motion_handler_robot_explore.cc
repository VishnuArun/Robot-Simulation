/**
 * @file motion_handler_robot_explore.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 *
 */
#include "src/motion_handler_robot_explore.h"


NAMESPACE_BEGIN(csci3081);

Pose MotionHandlerRobotExplore::UpdateSpeed(double lsr_L, double lsr_R) {
  double new_position_ = 0.001 * ((lsr_R > lsr_L) ? lsr_R : lsr_L);

  return Pose(clamp_vel(5 + (1-new_position_*lsr_R)),
  clamp_vel(5 + (1-new_position_*lsr_L)));
  }


NAMESPACE_END(csci3081);
