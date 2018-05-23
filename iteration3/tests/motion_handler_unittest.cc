


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

    // instiliaze robot speed
    robot = new csci3081::Robot();
    robot->set_robot_id(1);
    robot->SetLightSensorSensitivity(1200.0);

    LeftLightSensor = robot->GetLightSensorL();
    RightLightSensor = robot->GetLightSensorR();
    robot->set_robot_speed(5.0,5.0);


    light = new csci3081::Light();
    // intialize light_sensors

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
double CalculateTestVelocityFearL(double lsr_L) {
  double constant = 0.0000001;

  return clamp_vel(5 +
      constant*(lsr_L));

}

double CalculateTestVelocityFearR(double lsr_R) {
  double constant = 0.0000001;
  return clamp_vel(5 +
      constant*(lsr_R));

}

// Modifies robt_test velocity for eventual cross-check with actual robot
double CalculateTestVelocityExploreL(double lsr_L, double lsr_R) {
  double new_position_ = 0.001 * ((lsr_R > lsr_L) ? lsr_R : lsr_L);

  return clamp_vel(5 + (1-new_position_*lsr_R));
  }

double CalculateTestVelocityExploreR(double lsr_L, double lsr_R) {
  double new_position_ = 0.001 * ((lsr_R > lsr_L) ? lsr_R : lsr_L);

  return clamp_vel(5 + (1-new_position_*lsr_L));
  }



csci3081::Robot * robot;
csci3081::Robot * robot_test;
csci3081::LightSensor * RightLightSensor;
csci3081::LightSensor * LeftLightSensor;
csci3081::Light * light;

};
// Fear
TEST_F(UpdateVelocityTest, DirectConnection) {
  // We set robot ID to even so it is a fear robot
  robot->set_robot_id(2);
  robot->get_motion_handler()->set_velocity(4.0,4.0);
  EXPECT_DOUBLE_EQ(robot->get_motion_handler()->get_velocity().left,
  4.0)
  << "FAIL: Robot velocity not properly set on left wheel(Direct Connection)";

  EXPECT_DOUBLE_EQ(robot->get_motion_handler()->get_velocity().right,
  4.0)
  << "FAIL: Robot velocity not properly set on right wheel(Direct Connection)";

  // Case Robot and Light sensor Spawn on top one another
  robot->set_position(5.0,5.0);
  light->set_position(5.0,5.0);
  robot->CheckStimuli(light->get_type(),light->get_pose());
  // We check both left and right sensors
  EXPECT_DOUBLE_EQ(robot->get_motion_handler()->get_velocity().left,
  CalculateTestVelocityFearL(robot->GetLightSensorL()->get_reading()))
  << "FAIL: Robot velocity not properly set on leftt sensor when light spawned on top of sensor(Direct Connection)";
  EXPECT_DOUBLE_EQ(robot->get_motion_handler()->get_velocity().right,
  CalculateTestVelocityFearR(robot->GetLightSensorR()->get_reading()))
  << "FAIL: Robot velocity not properly set on right sensor when light spawned on top of sensor(Direct Connection)";

  robot->GetLightSensorL()->set_reading(0.0);
  robot->GetLightSensorR()->set_reading(0.0);
  // Near Proximity stimuli
  robot->set_position(5.0,5.0);
  light->set_position(15.0,15.0);
  robot->CheckStimuli(light->get_type(),light->get_pose());

  EXPECT_DOUBLE_EQ(robot->get_motion_handler()->get_velocity().left,
  CalculateTestVelocityFearL(robot->GetLightSensorL()->get_reading()))
  <<"FAIL: Nearby light not properly detected by left sensor(Direct Connection)";
  EXPECT_DOUBLE_EQ(robot->get_motion_handler()->get_velocity().right,
  CalculateTestVelocityFearR(robot->GetLightSensorR()->get_reading()))
    <<"FAIL: Nearby light not properly detected by right sensor(Direct Connection)";
  robot->GetLightSensorL()->set_reading(0.0);
  robot->GetLightSensorR()->set_reading(0.0);

  // Far away stimuli
  robot->set_position(250.0,250.0);
  light->set_position(15.0,15.0);
  robot->CheckStimuli(light->get_type(),light->get_pose());

  EXPECT_DOUBLE_EQ(robot->get_motion_handler()->get_velocity().left,
  CalculateTestVelocityFearL(robot->GetLightSensorL()->get_reading()))
  <<"FAIL: Far away stimuli not properly detected by left sensor(Direct Connection)";
  EXPECT_DOUBLE_EQ(robot->get_motion_handler()->get_velocity().right,
  CalculateTestVelocityFearR(robot->GetLightSensorR()->get_reading()))
  <<"FAIL: Far away stimuli not properly detected by right sensor(Direct Connection)";
  // Maximum distance stimuli
  robot->set_position(1024.0,768.0);
  light->set_position(1.0,1.0);
  robot->CheckStimuli(light->get_type(),light->get_pose());

  EXPECT_DOUBLE_EQ(robot->get_motion_handler()->get_velocity().left,
  CalculateTestVelocityFearL(robot->GetLightSensorL()->get_reading()))
  <<"FAIL: Edge case stimuli not properly detected by left sensor(Direct Connection)";
  EXPECT_DOUBLE_EQ(robot->get_motion_handler()->get_velocity().right,
  CalculateTestVelocityFearR(robot->GetLightSensorR()->get_reading()))
  <<"FAIL: Edge case stimuli not properly detected by right sensor(Direct Connection)";

  // Multiple lights: we call robots CheckStimuli mulitple times and change
  // the light position between calls. This simulates multiple lights
  robot->set_position(5.0,5.0);
  light->set_position(5.0,5.0);
  robot->CheckStimuli(light->get_type(),light->get_pose());

  light->set_position(15.0,15.0);
  robot->CheckStimuli(light->get_type(),light->get_pose());

  // Two lights
  EXPECT_DOUBLE_EQ(robot->get_motion_handler()->get_velocity().left,
  CalculateTestVelocityFearL(robot->GetLightSensorL()->get_reading()))
  <<"FAIL:Motion handler failed to properly set velocity of left wheel  after detecting two lights(Direct Connection)";

  EXPECT_DOUBLE_EQ(robot->get_motion_handler()->get_velocity().right,
  CalculateTestVelocityFearR(robot->GetLightSensorR()->get_reading()))
  <<"FAIL: Motion handler failed to properly set velocity of right wheeel after detecting two lights(Direct Connection)";

  // Far away stimuli
  light->set_position(1024.0,768.0);
  robot->CheckStimuli(light->get_type(),light->get_pose());

  // Three lights
  EXPECT_DOUBLE_EQ(robot->get_motion_handler()->get_velocity().left,
  CalculateTestVelocityFearL(robot->GetLightSensorL()->get_reading()))
  <<"FAIL: Motion handler failed to properly set velocity of left wheel  after detecting  three lights(Direct Connection)";
  EXPECT_DOUBLE_EQ(robot->get_motion_handler()->get_velocity().right,
  CalculateTestVelocityFearR(robot->GetLightSensorR()->get_reading()))
  <<"FAIL: Motion handler failed to properly set velocity of right wheel after detecting three lights(Direct Connection)";
  light->set_position(500.0,500.0);
  robot->CheckStimuli(light->get_type(),light->get_pose());

  // Four lights
  EXPECT_DOUBLE_EQ(robot->get_motion_handler()->get_velocity().left,
  CalculateTestVelocityFearL(robot->GetLightSensorL()->get_reading()))
  <<"FAIL: Motion handler failed to properly set velocity of left wheel after detecting  four lights(Direct Connection)";

  EXPECT_DOUBLE_EQ(robot->get_motion_handler()->get_velocity().right,
  CalculateTestVelocityFearR(robot->GetLightSensorR()->get_reading()))
  <<"FAIL: Motion handler failed to properly set velocity of right wheel after detecting four lights (Direct Connection)";

  light->set_position(600.0,600.0);
  robot->CheckStimuli(light->get_type(),light->get_pose());

  // Five lights
  EXPECT_DOUBLE_EQ(robot->get_motion_handler()->get_velocity().left,
  CalculateTestVelocityFearL(robot->GetLightSensorL()->get_reading()))
  <<"FAIL: Motion handler failed to properly set velocity of left wheel after detecting five lights(Direct Connection)";

  EXPECT_DOUBLE_EQ(robot->get_motion_handler()->get_velocity().right,
  CalculateTestVelocityFearR(robot->GetLightSensorR()->get_reading()))
  <<"FAIL: Motion handler failed to properly set velocity of right wheel after detecting  five lights(Direct Connection)";


}

TEST_F(UpdateVelocityTest, CrossConnection) {
  // Explore Robot
  robot->set_robot_id(1);
  robot->get_motion_handler()->set_velocity(4.0,4.0);
  EXPECT_DOUBLE_EQ(robot->get_motion_handler()->get_velocity().left,
  4.0)
  <<"FAIL: Robot failed to set wheel velocity left (CrossConnection)";
  EXPECT_DOUBLE_EQ(robot->get_motion_handler()->get_velocity().right,
  4.0)
  <<"FAIL: Robot failed to set wheel velocity left (CrossConnection)";

  // Case Robot and Light sensor Spawn on top one another
  robot->set_position(5.0,5.0);
  light->set_position(5.0,5.0);
  robot->CheckStimuli(light->get_type(),light->get_pose());

  EXPECT_DOUBLE_EQ(robot->get_motion_handler()->get_velocity().left,
  CalculateTestVelocityExploreL(robot->GetLightSensorL()->get_reading(),
  robot->GetLightSensorR()->get_reading()))
  <<"FAIL: Motion Handler failed to set velocity of left wheel after light spawed on top of robot (edgeCase) CrossConnection";

  EXPECT_DOUBLE_EQ(robot->get_motion_handler()->get_velocity().right,
  CalculateTestVelocityExploreR(robot->GetLightSensorL()->get_reading(),
  robot->GetLightSensorR()->get_reading()))
  <<"FAIL: Motion Handler failed to set velocity of right wheel after light spawed on top of robot (edgeCase) CrossConnection";


  robot->GetLightSensorL()->set_reading(0.0);
  robot->GetLightSensorR()->set_reading(0.0);
  // Near Proximity stimuli
  robot->set_position(5.0,5.0);
  light->set_position(15.0,15.0);
  robot->CheckStimuli(light->get_type(),light->get_pose());

  EXPECT_DOUBLE_EQ(robot->get_motion_handler()->get_velocity().left,
  CalculateTestVelocityExploreL(robot->GetLightSensorL()->get_reading(),
  robot->GetLightSensorR()->get_reading()))
  <<"FAIL: Motion Handler failed to set velocity of left wheel after light spawed near robot CrossConnection";


  EXPECT_DOUBLE_EQ(robot->get_motion_handler()->get_velocity().right,
  CalculateTestVelocityExploreR(robot->GetLightSensorL()->get_reading(),
  robot->GetLightSensorR()->get_reading()))
  <<"FAIL: Motion Handler failed to set velocity of right wheel after light spawed near robot CrossConnection";


  robot->GetLightSensorL()->set_reading(0.0);
  robot->GetLightSensorR()->set_reading(0.0);
  // Far away stimuli
  robot->set_position(250.0,250.0);
  light->set_position(15.0,15.0);
  robot->CheckStimuli(light->get_type(),light->get_pose());

  EXPECT_DOUBLE_EQ(robot->get_motion_handler()->get_velocity().left,
  CalculateTestVelocityExploreL(robot->GetLightSensorL()->get_reading(),
  robot->GetLightSensorR()->get_reading()))
  <<"FAIL: Motion Handler failed to set velocity of left wheel after light spawed far away robot CrossConnection";


  EXPECT_DOUBLE_EQ(robot->get_motion_handler()->get_velocity().right,
  CalculateTestVelocityExploreR(robot->GetLightSensorL()->get_reading(),
  robot->GetLightSensorR()->get_reading()))
  <<"FAIL: Motion Handler failed to set velocity of right wheel after light spawed far away robot CrossConnection";


  // Multiple lights
  robot->set_position(5.0,5.0);
  light->set_position(5.0,5.0);
  robot->CheckStimuli(light->get_type(),light->get_pose());




  light->set_position(15.0,15.0);
  robot->CheckStimuli(light->get_type(),light->get_pose());

  // Two lights
  EXPECT_DOUBLE_EQ(robot->get_motion_handler()->get_velocity().left,
  CalculateTestVelocityExploreL(robot->GetLightSensorL()->get_reading(),
  robot->GetLightSensorR()->get_reading()))
  <<"FAIL: Motion Handler failed to set velocity of left wheel after two light spawed, robot CrossConnection";



  EXPECT_DOUBLE_EQ(robot->get_motion_handler()->get_velocity().right,
  CalculateTestVelocityExploreR(robot->GetLightSensorL()->get_reading(),
  robot->GetLightSensorR()->get_reading()))
  <<"FAIL: Motion Handler failed to set velocity of right wheel after two light spawed, robot CrossConnection";


  // Far away stimuli
  light->set_position(150.0,150.0);
  robot->CheckStimuli(light->get_type(),light->get_pose());

  // Three lights
  EXPECT_DOUBLE_EQ(robot->get_motion_handler()->get_velocity().left,
  CalculateTestVelocityExploreL(robot->GetLightSensorL()->get_reading(),
  robot->GetLightSensorR()->get_reading()))
  <<"FAIL: Motion Handler failed to set velocity of right wheel after three light spawed, robot CrossConnection";


  EXPECT_DOUBLE_EQ(robot->get_motion_handler()->get_velocity().right,
  CalculateTestVelocityExploreR(robot->GetLightSensorL()->get_reading(),
  robot->GetLightSensorR()->get_reading()))
  <<"FAIL: Motion Handler failed to set velocity of left wheel after three light spawed, robot CrossConnection";


  light->set_position(500.0,500.0);
  robot->CheckStimuli(light->get_type(),light->get_pose());

  // Four lights
  EXPECT_DOUBLE_EQ(robot->get_motion_handler()->get_velocity().left,
  CalculateTestVelocityExploreL(robot->GetLightSensorL()->get_reading(),
  robot->GetLightSensorR()->get_reading()))
  <<"FAIL: Motion Handler failed to set velocity of left wheel after four light spawed, robot CrossConnection";


  EXPECT_DOUBLE_EQ(robot->get_motion_handler()->get_velocity().right,
  CalculateTestVelocityExploreR(robot->GetLightSensorL()->get_reading(),
  robot->GetLightSensorR()->get_reading()))
  <<"FAIL: Motion Handler failed to set velocity of left wheel after four light spawed, robot CrossConnection";


  light->set_position(600.0,600.0);
  robot->CheckStimuli(light->get_type(),light->get_pose());

  // Five lights
  EXPECT_DOUBLE_EQ(robot->get_motion_handler()->get_velocity().left,
  CalculateTestVelocityExploreL(robot->GetLightSensorL()->get_reading(),
  robot->GetLightSensorR()->get_reading()))
  <<"FAIL: Motion Handler failed to set velocity of left wheel after five light spawed, robot CrossConnection";


  EXPECT_DOUBLE_EQ(robot->get_motion_handler()->get_velocity().right,
  CalculateTestVelocityExploreR(robot->GetLightSensorL()->get_reading(),
  robot->GetLightSensorR()->get_reading()))
  <<"FAIL: Motion Handler failed to set velocity of left wheel after five light spawed, robot CrossConnection";


}

TEST_F(UpdateVelocityTest, Very_Hungry){
  // Check Very hungry
  robot->set_robot_id(1);
  robot->set_position(5.0,5.0);
  robot->get_motion_handler()->set_velocity(4.0,4.0);
  robot->set_hungry_time(-99999);

  light->set_position(5.0,5.0);
  robot->CheckStimuli(light->get_type(),light->get_pose());

  // We expect the robot to not change velocity in presence of light.
  EXPECT_DOUBLE_EQ(robot->get_motion_handler()->get_velocity().left,4.0)
  <<"FAIL: Robot not properly ignoring light when very hungry, left wheel speed incorrect (Fear Robot)";
  EXPECT_DOUBLE_EQ(robot->get_motion_handler()->get_velocity().right,4.0)
  <<"FAIL: Robot not properly ignoring light when very hungry, right wheel speed incorrect (Fear Robot)";

  robot->set_robot_id(0);
  robot->CheckStimuli(light->get_type(),light->get_pose());

  // We expect the robot to not change velocity in presence of light.
  EXPECT_DOUBLE_EQ(robot->get_motion_handler()->get_velocity().left,4.0)
  <<"FAIL: Robot not properly ignoring light when very hungry, left wheel speed incorrect (Explore Robot)";

  EXPECT_DOUBLE_EQ(robot->get_motion_handler()->get_velocity().right,4.0)
  <<"FAIL: Robot not properly ignoring light when very hungry, right wheel speed incorrect (Explore Robot)";


  // Check Very hungry At max robot speed
  robot->set_position(5.0,5.0);
  robot->get_motion_handler()->set_velocity(10.0,10.0);
  robot->set_hungry_time(-99999);

  light->set_position(5.0,5.0);
  robot->CheckStimuli(light->get_type(),light->get_pose());

  EXPECT_DOUBLE_EQ(robot->get_motion_handler()->get_velocity().left,10.0)
  <<"FAIL: Robot not properly ignoring light when very hungry, left wheel speed incorrect. Robot intially at max speed";

  EXPECT_DOUBLE_EQ(robot->get_motion_handler()->get_velocity().right,10.0)
  <<"FAIL: Robot not properly ignoring light when very hungry, right wheel speed incorrect. Robot intially at max speed";



}

#endif
