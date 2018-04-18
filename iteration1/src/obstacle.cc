/**
 * @file obstacle.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/obstacle.h"
#include "src/params.h"


/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Constructors/Destructor
 ******************************************************************************/
// Extra arguments included for reverse action upon collision
Obstacle::Obstacle()  :
    motion_handler_(this),
    motion_behavior_(this),
    ArcReverse_(false),
    ArcTime_(0) {
  set_color(OBSTACLE_COLOR);
  set_pose(OBSTACLE_POSITION);
  set_radius(OBSTACLE_RADIUS);
  set_type(kObstacle);
  // Previous Bug GCC required extra semicolon before line
  motion_handler_.set_velocity(OBSTACLE_SPEED, OBSTACLE_SPEED);
}

// Reset all attributes when new obstacle requested
void Obstacle::Reset() {
  // set_pose(OBSTACLE_POSITION);
  set_pose(SetPoseRandomly());
  set_radius(RandomRadius());
  set_color(OBSTACLE_COLOR);
  set_type(kObstacle);
  sensor_touch_->Reset();
  motion_handler_.set_velocity(OBSTACLE_SPEED, OBSTACLE_SPEED);
  ArcReverse_ = false;
  ArcTime_ = 0;
}

// Controls how Obstacle changes over time via discrete timesteps
void Obstacle::TimestepUpdate(unsigned int dt) {
  // Update heading as indicated by touch sensor



  // Reset Sensor for next cycle
  sensor_touch_->Reset();

  // Additional Arguments for Arc reverse behavior
  if (ArcReverse_ & (ArcTime_ == 0)) {
    this->RelativeChangeHeading(180);
    ArcTime_ = 5;
    ArcReverse_ = false;
  }

  if ((ArcTime_ < 4) & (ArcTime_ > 1)) {
      motion_handler_.set_velocity(motion_handler_.get_velocity().left + 0.2,
        (motion_handler_.get_velocity().right - 0.2));

      motion_behavior_.UpdatePose(ArcTime_, motion_handler_.get_velocity());
  }
  if (ArcTime_ == 1) {
    this->RelativeChangeHeading(20);
    motion_handler_.set_velocity(OBSTACLE_SPEED, OBSTACLE_SPEED);
  }
  if (ArcTime_ > 0) {
    ArcTime_--;
  }

  // Use velocity and position to update position
  motion_behavior_.UpdatePose(dt, motion_handler_.get_velocity());
} /* TimestepUpdate() */

// Obstacle Collision Handling and ArcReverse trigger
void Obstacle::HandleCollision(EntityType object_type, ArenaEntity * object) {
  sensor_touch_->HandleCollision(object_type, object);

  switch (object_type) {
    case (kObstacle):  // set zero velocity
    case (kRightWall):
    case (kLeftWall):
    case (kTopWall):
    case (kBottomWall):
    case (kBase):
    case (kRobot):
    // Flag communocates with timestep update
    ArcReverse_ = true;
    break;

    default : break;
  }
}

NAMESPACE_END(csci3081);
