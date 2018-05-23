


#include <gtest/gtest.h>
#include <src/arena.h>
#include <src/light_sensor.h>
#include <src/light.h>
#include <src/params.h>
#include <src/robot.h>
#include <src/wheel_velocity.h>
#include <src/arena_params.h>
#include <src/arena_entity.h>
#include <src/params.h>
#include <src/motion_handler_robot.h>
#include <cmath>


#ifndef FEEDBACK_MOTION_HANDLER_TEST

class UpdateVelocityTest : public ::testing::Test{


protected:
  virtual void SetUp() {
    csci3081::arena_params aparams;
    arena = new csci3081::Arena(&aparams);
    // instiliaze robot speed
    robot = arena->robot();
    // Used for comparison evaluation
    robot_test = arena->robot();
    // intialize light_sensors
    light_sensor_L = new csci3081::LightSensor({100,100});
    light_sensor_R = new csci3081::LightSensor({200,200});
    // intialize robot speed
    robot->set_speed(2,2);
    robot_test->set_speed(5,3);


  }

  /**
  *@brief clamp_vel : adapted from motion handler robot
  */
  double clamp_vel(double vel) {
  // @TODO: don't go backwards
  double clamped = 0.0;
  if (vel > 0) {
    clamped = (vel > 10) ?
              10: vel;
  } else {
    // modified to reset to some instial speed for test rather than 0
    clamped = 1;
  }
  return clamped;
} /* clamp_vel() */



// Modifies robt_test velocity for eventual cross-check with actual robot
void CalculateTestVelocityFear( double lsr_L, double lsr_R) {
  double constant = 0.001;
    robot_test->set_speed(
      clamp_vel(robot_test->get_motion_handler().get_velocity().left +
      constant*(1000-lsr_R)),clamp_vel(robot_test->get_motion_handler().get_velocity().right
    + constant*(1000-lsr_L)));
    return;



}
void CalculateTestVelocityExploratory ( double lsr_L, double lsr_R) {
    double constant = 0.001 * ((lsr_R > lsr_L) ? lsr_R : lsr_L);
    robot_test->set_speed(
      clamp_vel(5 + (1-constant*lsr_L)),clamp_vel(5+)
    + (1-constant*lsr_R)));


    return;
  }



csci3081::Arena * arena;
csci3081::Robot * robot;
csci3081::Robot * robot_test;
csci3081::LightSensor * light_sensor_L;
csci3081::LightSensor * light_sensor_R;

};

TEST_F(UpdateVelocityTest, DirectConnection) {

  // Both sensors view one lamp at coordinates 15 15 with robot fear persona
  std::cout<<robot_test->get_motion_handler().get_velocity().left;
  light_sensor_R-> CalculateReading({15,15});
  light_sensor_L->CalculateReading({15,15});
  robot->UpdateSpeed(light_sensor_L->get_reading(),light_sensor_R->get_reading(),'F');
  CalculateTestVelocityFear(light_sensor_L->get_reading(),light_sensor_R->get_reading());
  EXPECT_DOUBLE_EQ(robot->get_motion_handler().get_velocity().left,
  robot_test->get_motion_handler().get_velocity().left)
  << "FAIL : Mismatch left wheel velocities";

  EXPECT_DOUBLE_EQ(robot->get_motion_handler().get_velocity().right,
  robot_test->get_motion_handler().get_velocity().right)
  << "FAIL : Mismatch left wheel velocities";


  // Two lamps at coordinates 15 15 and 35 35 with robot feear persona
  // reset robots and sensors
  light_sensor_L->set_reading(0.0);
  light_sensor_R->set_reading(0.0);
  robot->set_speed(1.0,1.0);
  robot_test->set_speed(1.0,1.0);
  light_sensor_R-> CalculateReading({15,15});
  light_sensor_L->CalculateReading({15,15});
  robot->UpdateSpeed(light_sensor_L->get_reading(),light_sensor_R->get_reading(),'F');
  CalculateTestVelocityFear(light_sensor_L->get_reading(),light_sensor_R->get_reading());
  // now visiting next lamp

  light_sensor_R-> CalculateReading({30,30});
  light_sensor_L->CalculateReading({30,30});
  robot->UpdateSpeed(light_sensor_L->get_reading(),light_sensor_R->get_reading(),'F');
  CalculateTestVelocityFear(light_sensor_L->get_reading(),light_sensor_R->get_reading());

  EXPECT_DOUBLE_EQ(robot->get_motion_handler().get_velocity().left,
  robot_test->get_motion_handler().get_velocity().left)
  << "FAIL : Mismatch left wheel velocities";

  EXPECT_DOUBLE_EQ(robot->get_motion_handler().get_velocity().right,
  robot_test->get_motion_handler().get_velocity().right)
  << "FAIL : Mismatch left wheel velocities";

}

TEST_F(UpdateVelocityTest, CrossConnection) {

  // Both sensors view one lamp at coordinates 15 15 with robot fear persona
  light_sensor_R-> CalculateReading({15,15});
  light_sensor_L->CalculateReading({15,15});
  robot->UpdateSpeed(light_sensor_L->get_reading(),light_sensor_R->get_reading(),'E');
  CalculateTestVelocityExploratory(light_sensor_L->get_reading(),light_sensor_R->get_reading());
  EXPECT_DOUBLE_EQ(robot->get_motion_handler().get_velocity().left,
  robot_test->get_motion_handler().get_velocity().left)
  << "FAIL : Mismatch left wheel velocities";

  EXPECT_DOUBLE_EQ(robot->get_motion_handler().get_velocity().right,
  robot_test->get_motion_handler().get_velocity().right)
  << "FAIL : Mismatch left wheel velocities";


  // Two lamps at coordinates 15 15 and 35 35 with robot explore persona
  // reset robots and sensors
  light_sensor_L->set_reading(0.0);
  light_sensor_R->set_reading(0.0);
  robot->set_speed(1.0,1.0);
  robot_test->set_speed(1.0,1.0);
  light_sensor_R-> CalculateReading({15,15});
  light_sensor_L->CalculateReading({15,15});
  robot->UpdateSpeed(light_sensor_L->get_reading(),light_sensor_R->get_reading(),'E');
  CalculateTestVelocityExploratory(light_sensor_L->get_reading(),light_sensor_R->get_reading());
  // now visiting next lamp

  light_sensor_R-> CalculateReading({30,30});
  light_sensor_L->CalculateReading({30,30});
  robot->UpdateSpeed(light_sensor_L->get_reading(),light_sensor_R->get_reading(),'E');
  CalculateTestVelocityExploratory(light_sensor_L->get_reading(),light_sensor_R->get_reading());

  EXPECT_DOUBLE_EQ(robot->get_motion_handler().get_velocity().left,
  robot_test->get_motion_handler().get_velocity().left)
  << "FAIL : Mismatch left wheel velocities";

  EXPECT_DOUBLE_EQ(robot->get_motion_handler().get_velocity().right,
  robot_test->get_motion_handler().get_velocity().right)
  << "FAIL : Mismatch left wheel velocities";

}







#endif
