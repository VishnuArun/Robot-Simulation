/**
 * @file motion_handler_robot_fear.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 *
 */
#ifndef SRC_MOTION_HANDLER_ROBOT_FEAR_H_
#define SRC_MOTION_HANDLER_ROBOT_FEAR_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <cassert>
#include <iostream>

#include "src/common.h"
#include "src/motion_handler_robot.h"
#include "src/motion_handler.h"



/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/**
 * @brief class extends from MotionHandlerRobot to provide specific behaviors
 for robots motion
 *
 * Included function that overrides UpdateSpeed so Robot can exhibit this
 classes behaviors
 */

class MotionHandlerRobotFear : public MotionHandlerRobot {
 public:
  explicit MotionHandlerRobotFear(ArenaMobileEntity * ent)
      : MotionHandlerRobot(ent) {}

  MotionHandlerRobotFear(const MotionHandlerRobotFear& other) = default;
  MotionHandlerRobotFear& operator=(const MotionHandlerRobotFear& other)
  = default;


  /**
  * @ brief Update Speed implemented for Fear behavior
  */
Pose UpdateSpeed(double lsr_L, double lsr_R) override;
};
NAMESPACE_END(csci3081);
#endif  // SRC_MOTION_HANDLER_ROBOT_FEAR_H_
