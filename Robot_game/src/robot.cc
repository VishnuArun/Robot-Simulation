/**
 * @file robot.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/robot.h"
#include "src/params.h"
#include "src/base.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Constructors/Destructor
 ******************************************************************************/
Robot::Robot() :
    motion_handler_(this),
    motion_behavior_(this),
    lives_(9),
    mercy_mode_(false),
    mercy_timer_(0) {
  set_type(kRobot);
  set_color(ROBOT_COLOR);
  set_pose(ROBOT_INIT_POS);
  set_radius(ROBOT_RADIUS);
}
/*******************************************************************************
 * Member Functions
 ******************************************************************************/
void Robot::TimestepUpdate(unsigned int dt) {
  // Update heading as indicated by touch sensor
  motion_handler_.UpdateVelocity();

  // Use velocity and position to update position
  motion_behavior_.UpdatePose(dt, motion_handler_.get_velocity());

  // Reset Sensor for next cycle
  sensor_touch_->Reset();

  // Mercy invisibilty

  if (mercy_mode_ & (mercy_timer_ == 0)) {
    mercy_timer_ = 60;
  }
  if (mercy_timer_ > 0) {
    // Color switchcing functionality
    if (mercy_timer_ % 5 == 0) {
      set_color(ROBOT_COLOR_MERCY);
    }
    if (mercy_timer_ % 3 == 0) {
      set_color(ROBOT_COLOR);
    }
    // Deactivate mercy invincibilty at last timestep
    if (mercy_timer_ == 1) {
      mercy_mode_ = false;
    }
    mercy_timer_--;
  }
} /* TimestepUpdate() */

void Robot::Reset() {
  set_pose(ROBOT_INIT_POS);
  motion_handler_.set_max_speed(ROBOT_MAX_SPEED);
  motion_handler_.set_max_angle(ROBOT_MAX_ANGLE);
  sensor_touch_->Reset();
  set_lives(9);
  mercy_timer_ = 0;
  mercy_mode_ = false;
} /* Reset() ;*/

void Robot::HandleCollision(EntityType object_type, ArenaEntity * object) {
  sensor_touch_->HandleCollision(object_type, object);

  switch (object_type) {
      // Adpated for Mercy invincibilty conditions
    case (kRightWall):
    case (kLeftWall):
    case (kTopWall):
    case (kBottomWall):
    // Implement mercy invisibilty function
    case (kObstacle):
    motion_handler_.set_velocity(0.0, 0.0);

    if (mercy_mode_ == false) {
      lives_--;
    }
    mercy_mode_ = true;

    // call mercy function

    break;

    case (kBase):
    motion_handler_.set_velocity(0.0, 0.0);
    // Set Base color R -> G
    object->set_color({0, 100, 100});
    // Type cast a entity as a base
    static_cast <Base*>(object)->set_captured(true);
    break;
    default : break;
  }
}

void Robot::IncreaseSpeed() {
  motion_handler_.IncreaseSpeed();
}
void Robot::DecreaseSpeed() {
  motion_handler_.DecreaseSpeed();
}
void Robot::TurnRight() {
  motion_handler_.TurnRight();
}
void Robot::TurnLeft() {
  motion_handler_.TurnLeft();
}

NAMESPACE_END(csci3081);
