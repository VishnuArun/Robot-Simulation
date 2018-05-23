/**
 * @file light.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 *
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/light.h"
#include "src/params.h"


/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Constructors/Destructor
 ******************************************************************************/
// Extra arguments included for reverse action upon collision
Light::Light()  :
    motion_handler_(this),
    motion_behavior_(this),
    arc_reverse_(false),
    arc_time_(0) {
  set_color(LIGHT_COLOR);
  set_pose(LIGHT_POSITION);
  set_radius(LIGHT_RADIUS);
  set_type(kLight);
  // Previous Bug GCC required extra semicolon before line
  motion_handler_.set_velocity(LIGHT_SPEED, LIGHT_SPEED);
}

// Reset all attributes when new light requested
void Light::Reset() {
  // set_pose(LIGHT_POSITION);
  set_pose(SetPoseRandomly());
  set_radius(RandomRadius());
  set_color(LIGHT_COLOR);
  set_type(kLight);
  sensor_touch_->Reset();
  motion_handler_.set_velocity(LIGHT_SPEED, LIGHT_SPEED);
  arc_reverse_ = false;
  arc_time_ = 0;
}

// Controls how Light changes over time via discrete timesteps
void Light::TimestepUpdate(unsigned int dt) {
  // Update heading as indicated by touch sensor



  // Reset Sensor for next cycle
  sensor_touch_->Reset();

  // Additional Arguments for Arc reverse behavior
  if (arc_reverse_ & (arc_time_ == 0)) {
    this->RelativeChangeHeading(180);
    arc_time_ = 5;
    arc_reverse_ = false;
  }

  if ((arc_time_ < 4) & (arc_time_ > 1)) {
      motion_handler_.set_velocity(motion_handler_.get_velocity().left + 0.2,
        (motion_handler_.get_velocity().right - 0.2));

      motion_behavior_.UpdatePose(arc_time_, motion_handler_.get_velocity());
  }
  if (arc_time_ == 1) {
    this->RelativeChangeHeading(20);
    motion_handler_.set_velocity(LIGHT_SPEED, LIGHT_SPEED);
  }
  if (arc_time_ > 0) {
    arc_time_--;
  }

  // Use velocity and position to update position
  motion_behavior_.UpdatePose(dt, motion_handler_.get_velocity());
} /* TimestepUpdate() */

// Light Collision Handling and ArcReverse trigger
void Light::HandleCollision(EntityType object_type, ArenaEntity * object) {
  switch (object_type) {
    case (kLight):  // set zero velocity
    case (kRightWall):
    case (kLeftWall):
    case (kTopWall):
    case (kBottomWall):
    case (kFood):
    sensor_touch_->HandleCollision(object_type, object);
    // Flag communocates with timestep update
    arc_reverse_ = true;
    break;
    case (kRobot):
    break;

    default : break;
  }
}

NAMESPACE_END(csci3081);
