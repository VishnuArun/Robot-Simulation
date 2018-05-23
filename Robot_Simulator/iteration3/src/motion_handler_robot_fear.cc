/**
 * @file motion_handler_robot_fear.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 *
 */
#include "src/motion_handler_robot_fear.h"


NAMESPACE_BEGIN(csci3081);

Pose MotionHandlerRobotFear::UpdateSpeed(double lsr_L, double lsr_R) {
  return Pose(clamp_vel(5 + 0.0000001*lsr_L),
  clamp_vel(5 +0.0000001*lsr_R));
  }

NAMESPACE_END(csci3081);
