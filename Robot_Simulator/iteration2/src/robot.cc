/**
 * @file robot.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 *
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/robot.h"
#include "src/params.h"
#include "src/food.h"


/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Constructors/Destructor
 ******************************************************************************/
Robot::Robot() :
    motion_handler_(new MotionHandlerRobot(this)),
    motion_behavior_(this),
    lives_(9),
    arc_reverse_(false),
    arc_time_(0),
    consumed_food_(false),
    hungry_timer_(HUNGRY_TIME),
    light_sensor_R(UpdateSensorPosition('R')),
    light_sensor_L(UpdateSensorPosition('L')),
    food_sensor_R(UpdateSensorPosition('R')),
    food_sensor_L(UpdateSensorPosition('L')) {
  set_type(kRobot);
  set_color(ROBOT_COLOR);
  set_pose(ROBOT_INIT_POS);
  set_radius(ROBOT_RADIUS);

  // Intial Velocity
  motion_handler_->set_velocity(5.0, 5.0);
}
/*******************************************************************************
 * Member Functions
 ******************************************************************************/
void Robot::TimestepUpdate(unsigned int dt) {
  // Update heading as indicated by touch sensor
  motion_handler_->UpdateVelocity();

  // Use velocity and position to update position
  motion_behavior_.UpdatePose(dt, motion_handler_->get_velocity());

  // Reset Sensor for next cycle
  sensor_touch_->Reset();
  // light_sensor_L.Reset();
  // light_sensor_R.Reset();
  light_sensor_L.set_pose(UpdateSensorPosition('L'));
  light_sensor_R.set_pose(UpdateSensorPosition('R'));

  light_sensor_L.Reset();
  light_sensor_R.Reset();

  food_sensor_L.set_pose(UpdateSensorPosition('L'));
  food_sensor_R.set_pose(UpdateSensorPosition('R'));

  // Hungry Mode
  hungry_timer_--;
  if (hungry_timer_ <= 0) {
    if (consumed_food_) {
      hungry_timer_ = HUNGRY_TIME;
      consumed_food_ = false;
      set_color(ROBOT_COLOR);
    } else {
        if ((-1*hungry_timer_) % 2 == 0) {
          set_color(ROBOT_COLOR_HUNGRY);
    } else {
        set_color(ROBOT_COLOR);
    }
  }
  }



  // Additional Arguments for Arc reverse behavior

  if (arc_reverse_ & (arc_time_ == 0)) {
    this->RelativeChangeHeading(180);
    arc_time_ = 5;
    arc_reverse_ = false;
  }

  if ((arc_time_ < 4) & (arc_time_ > 1)) {
      motion_handler_->set_velocity(motion_handler_->get_velocity().left + 0.2,
        (motion_handler_->get_velocity().right - 0.2));

      motion_behavior_.UpdatePose(arc_time_, motion_handler_->get_velocity());
  }
  if (arc_time_ == 1) {
    this->RelativeChangeHeading(20);
    motion_handler_->set_velocity(ROBOT_MAX_SPEED, ROBOT_MAX_SPEED);
  }
  if (arc_time_ > 0) {
    arc_time_--;
  }
} /* TimestepUpdate() */
void Robot::Reset() {
  set_pose(SetPoseRandomly());
  motion_handler_->set_max_speed(ROBOT_INIT_SPEED);
  motion_handler_->set_max_angle(ROBOT_INIT_SPEED);
  sensor_touch_->Reset();
  set_lives(9);
  arc_time_ = 0;
  arc_reverse_ = false;
  light_sensor_L.Reset();
  light_sensor_R.Reset();
  food_sensor_R.Reset();
  food_sensor_L.Reset();
  set_color(ROBOT_COLOR);
  consumed_food_ = false;
  hungry_timer_ = HUNGRY_TIME;
} /* Reset() ;*/

/**
* @brief CheckStimuli is called by arena at each time step.
* This function looks at Robot ID and decides which robots exhibit fear or
* Exploratory behavior. This decision by looking at the parity of the robot
* ID and delegating all even ID's as fear and the rest as exploratory
*/
void Robot::CheckStimuli(EntityType type, Pose ent) {
  if (type == kLight) {
  light_sensor_L.CalculateReading(ent);
  light_sensor_R.CalculateReading(ent);

  if (robot_id_ % 2 == 0) {
    MotionHandlerRobot *RobotFear =
    new MotionHandlerRobotFear(this);
    Pose new_position_ = RobotFear->UpdateSpeed(light_sensor_L.get_reading(),
    light_sensor_R.get_reading());
    if (hungry_timer_ > (-1*4*HUNGRY_TIME)) {
      motion_handler_->set_velocity(new_position_.x, new_position_.y);
    }
  } else {
    MotionHandlerRobot *RobotExp =
    new MotionHandlerRobotExplore(this);
    Pose new_position_ = RobotExp->UpdateSpeed(light_sensor_L.get_reading(),
    light_sensor_R.get_reading());
    if (hungry_timer_ > (-1*4*HUNGRY_TIME)) {
      motion_handler_->set_velocity(new_position_.x, new_position_.y);
    }
  }
} else if (type == kFood) {
  food_sensor_L.CalculateReading(ent);
  food_sensor_R.CalculateReading(ent);
  double fsr_L = food_sensor_L.get_reading();
  double fsr_R = food_sensor_R.get_reading();
  if (((fsr_L > 500) | (fsr_R > 500))) {
    consumed_food_ = true;
  }
  if (hungry_timer_ < 0) {
    MotionHandlerRobot *RobotAggressive =
    new MotionHandlerRobotAggressive(this);
    Pose new_position_ = RobotAggressive->UpdateSpeed(fsr_L, fsr_R);
    motion_handler_->set_velocity(new_position_.x + 2.0, 2.0 + new_position_.y);
  }
} else {
  // do nothing
}
}


void Robot::HandleCollision(EntityType object_type, ArenaEntity * object) {
  switch (object_type) {
    case (kLight):

    // pass through lights
    break;
      // Adpated for Mercy invincibilty conditions
    case (kRightWall):
    case (kLeftWall):
    case (kTopWall):
    case (kBottomWall):
    // Implement mercy invisibilty function
    arc_reverse_ = true;
    sensor_touch_->HandleCollision(object_type, object);




    // call mercy function

    break;


    case (kFood):
    sensor_touch_->HandleCollision(object_type, object);


    // Type cast a entity as a food
    break;

    default : break;
  }
}

void Robot::IncreaseSpeed() {
  motion_handler_->IncreaseSpeed();
}
void Robot::DecreaseSpeed() {
  motion_handler_->DecreaseSpeed();
}
void Robot::TurnRight() {
  motion_handler_->TurnRight();
}
void Robot::TurnLeft() {
  motion_handler_->TurnLeft();
}


void Robot::set_robot_speed(double lw, double rw) {
  motion_handler_->set_velocity(lw, rw);
}

Pose Robot::SetPoseRandomly() {
  // Dividing arena into 19x14 grid. Each grid square is 50x50
  return {static_cast<double>((30 + (random() % 19) * 50)),
        static_cast<double>((30 + (random() % 14) * 50))};
}



NAMESPACE_END(csci3081);
