/**
 * @file robot.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 *
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
#include "src/motion_handler_robot_fear.h"
#include "src/motion_handler_robot_explore.h"
#include "src/motion_handler_robot_aggressive.h"
#include "src/motion_behavior_differential.h"
#include "src/entity_type.h"
#include "src/light_sensor.h"
#include "src/food_sensor.h"
#include "src/params.h"

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
 * behavior calculates the new pose foodd on wheel velocities.
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

  virtual ~Robot() = default;

  Robot(const Robot& other) = default;
  Robot& operator=(const Robot& other) = default;

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

  int get_robot_id() {return robot_id_;}

  void set_robot_id(int id) {robot_id_ = id;}

  // Newly added functions

  void set_robot_speed(double lw, double rw);


  /**
  *  @brief Calculates changes in pose foodd on elapsed time and wheel velocities.
  */
  MotionBehaviorDifferential get_motion_behavior() {return motion_behavior_;}

  MotionHandlerRobot * get_motion_handler() {return motion_handler_;}
  /**
  * @brief Update Sensors to be 40 degrees from heading in either side of
  * its body corresponding to L and R
  */
  Pose UpdateSensorPosition(char type) const {
    double pi = atan(1)*4;
    if (type == 'R') {
      double thetaL_ = (this)->get_heading() + (-40.0*pi/180.0);
      double R_x_ = (this)->get_radius()*cos(thetaL_) + (this)->get_pose().x;
      double R_y_ = (this)->get_radius()*sin(thetaL_) + (this)->get_pose().y;
      return {R_x_, R_y_};

    } else if ( type == 'L' ) {
      double thetaR_ = (this)->get_heading() + (40.0*pi/180.0);
      double L_x_ = (this)->get_radius()*cos(thetaR_) + (this)->get_pose().x;
      double L_y_ = (this)->get_radius()*sin(thetaR_) + (this)->get_pose().y;
      return {L_x_, L_y_};
    } else {
      assert(false);
      return {0.0, 0.0};
    }
  }

  /**
  * @brief Delegates behavior based on entity type and stimuli reading.
  *@brief Makes behavior decisions.
  */
  void CheckStimuli(EntityType type, Pose ent);


  /**
  * @brief Method for checking if robot is starving. Used to stop simulation
  */
  int GetStarvingStatus() {
    if (hungry_timer_ < (-1 * 4 * HUNGRY_TIME)) {
      return 1;
    } else {
      return 2;
    }
  }

  void SetLightSensorSensitivity(double d) {
    light_sensor_R.SetSensitivity(d);
    light_sensor_L.SetSensitivity(d);
  }

  void SetNoFoodFlag(bool b) {
    no_food_flag_ = b;
  }

  bool GetNoFoodFlag() {
    return no_food_flag_;
  }

  LightSensor * GetLightSensorL() {
    return &light_sensor_L;
  }

  LightSensor * GetLightSensorR() {
    return &light_sensor_R;
  }

  void set_hungry_time(int n) {
    hungry_timer_ = n;
  }

 private:
  /**
  *  @brief Used for mobile enity behaviors
  */
  MotionHandlerRobot * motion_handler_;

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
   @ brief Following Variables used for ArcReverse fucntion where
   Robot reverse in an arc upon contact
  */
  bool arc_reverse_;

  // @brief Variable acts as a counter for how long arc behavior lasts
  int arc_time_;

  // @brief Used to identify robots by assigning unique id's
  int robot_id_ = 0;
  // @brief Used to Check if Robot as consumed food
  bool consumed_food_;
  // @brief Count down timer for robot hunger
  int hungry_timer_;

  bool no_food_flag_ = false;

  Pose SetPoseRandomly();
  // @brief Right light sensor for robot
  LightSensor light_sensor_R;
  // @brief Left light sensor for robot
  LightSensor light_sensor_L;
  // @brief Right food sensor for robot
  FoodSensor food_sensor_R;
  // @brief Left food sensor for robot
  FoodSensor food_sensor_L;
};

NAMESPACE_END(csci3081);

#endif  // SRC_ROBOT_H_
