/**
 * @file robot.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_ROBOT_H_
#define SRC_ROBOT_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <string>

#include "src/arena_mobile_entity.h"
#include "src/common.h"
#include "src/motion_handler_robot.h"
#include "src/motion_behavior_differential.h"
#include "src/entity_type.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

class MotionBehaviorDifferential;

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * @brief Class representing a robot within the arena.
 *
 * Robots are composed of a motion handler, motion behavior, and touch sensor.
 * These classes interact to maintain the pose (position and heading) of the
 * robot. At each time step, the wheel velocities are used to calculate the
 * next pose of the robot. The handler manages the pose and user requests. The
 * behavior calculates the new pose based on wheel velocities.
 *
 * Robots can be controlled through keypress, which modify wheel velocities.
 *
 * The touch sensor is activated when the robot collides with an object.
 * The heading is modified after a collision to move the robot away from the
 * other object.
 */
class Robot : public ArenaMobileEntity {
 public:
  /**
   * @brief Constructor using initialization values from params.h.
   */

  Robot();

  /**
   * @brief Reset the Robot to a newly constructed state (needed for reset
   * button to work in GUI).
   */
  void Reset() override;

  /**
   * @brief Update the Robot's position and velocity after the specified
   * duration has passed.
   *
   * @param dt The # of timesteps that have elapsed since the last update.
   *
   */
  void TimestepUpdate(unsigned int dt) override;


  /**
   * @brief Handles the collision by setting the sensor to activated.
   */
  void HandleCollision(EntityType object_type, ArenaEntity * object = NULL);

  /**
   * @brief Get the name of the Robot for visualization and for debugging.
   */
  std::string get_name() const override { return "Robot"; }

  /**
   * @brief Command that comes from the controller, then is passed to handler.
   */
  void IncreaseSpeed();

  /**
  * @brief Command that comes from the controller, then is passed to handler.
  */
  void DecreaseSpeed();

  /**
  * @brief Command that comes from the controller, then is passed to handler.
  */
  void TurnRight();

  /**
  * @brief Command that comes from the controller, then is passed to handler.
  */
  void TurnLeft();


  int get_lives() { return lives_;}

  void set_lives(int l) {lives_ = l;}


  /**
  * @brief Manages pose and wheel velocities that change with time and collisions.
  */
  MotionHandlerRobot get_motion_handler() {return motion_handler_;}
  /**
  *  @brief Calculates changes in pose based on elapsed time and wheel velocities.
  */
  MotionBehaviorDifferential get_motion_behavior() {return motion_behavior_;}

 private:
  /**
  *  @brief Used for mobile enity behaviors
  */
  MotionHandlerRobot motion_handler_;
  /**
  *  @brief Used for mobile enity behaviors
  */
  MotionBehaviorDifferential motion_behavior_;
  /**
  *  @brief Lives are decremented when the robot collides with anything.
  *  @brief When all the lives are gone, the game is lost.
  */
  int lives_;

  /**
  * @brief Mercy mode auxillary variables
  */
  bool mercy_mode_;
  /**
  *  @brief mecy timer local variable used as counter for mercy mode
  */
  int mercy_timer_;
};

NAMESPACE_END(csci3081);

#endif  // SRC_ROBOT_H_
