
#include "src/arena.h"
#include "src/arena_params.h"
#include "src/arena_entity.h"
#include <gtest/gtest.h>
#include "src/light.h"
#include "src/light_sensor.h"
#include "src/params.h"
#include <cmath>

#ifndef LIGHT_SENSOR_TESTS

// We set up an on light sensor instance
class LightSensorTests : public ::testing::Test {
protected:
  virtual void SetUp(){

    light_sensor = new csci3081::LightSensor({10, 10});
  }

/** We will test our lightsensor against this funtion. This function should
simulate what a light_sensor would read given the pose of a light
*/
double TestCalculateReading(csci3081::Pose entity){
    double reading = light_sensor->get_reading();


    reading += (1200.0 / pow(1.08,(pow((pow((light_sensor->get_position().x - entity.x),2.0) +
              pow((light_sensor->get_position().y - entity.y),2.0)),0.5) - LIGHT_RADIUS)));

    // This control structure represents the saturation of the light sensor
    if (reading > 1000.0){
              return (1000.0);
            }
    else {
              return (reading);
            }

}
// We will creater a pointer instance to reference
csci3081::LightSensor * light_sensor;
};

// Intilaization tests
TEST_F(LightSensorTests, Constructor){
  EXPECT_DOUBLE_EQ(light_sensor->get_reading(),0.0)
  << "FAIL : Light constructor not properly intialized for default reading";
  EXPECT_DOUBLE_EQ(light_sensor->get_position().x,10)
  << "FAIL : Light constructor not properly intialized for x position";
  EXPECT_DOUBLE_EQ(light_sensor->get_position().y,10)
  << "FAIL : Light constructor not properly intialized for y position";
}

// Get/Set position function check
TEST_F(LightSensorTests, SetAndGetPosition){
  light_sensor->set_position(1.1,2.2);
  EXPECT_DOUBLE_EQ(light_sensor->get_position().x,1.1)
  << "FAIL : Light Get/Set not properly working for x coordinate";
  EXPECT_DOUBLE_EQ(light_sensor->get_position().y,2.2)
  << "FAIL : Light Get/Set not properly working for y coordinate";
  light_sensor->set_position(0.0,0.0);
  EXPECT_DOUBLE_EQ(light_sensor->get_position().x,0.0)
  << "FAIL : Light Get/Set not properly working for x coordinate";
  EXPECT_DOUBLE_EQ(light_sensor->get_position().y,0.0)
  << "FAIL : Light Get/Set not properly working for y coordinate";
}


// Get/Set reading function check
TEST_F(LightSensorTests, SetAndGetReading){
  light_sensor->set_reading(10.2);
  EXPECT_DOUBLE_EQ(light_sensor->get_reading(),10.2)
  << "FAIL : Light Get/Set not properly working for reading";
  light_sensor->set_reading(102.2);
  EXPECT_DOUBLE_EQ(light_sensor->get_reading(),102.2)
  << "FAIL : Light Get/Set not properly working for reading";
  light_sensor->set_reading(999.2);
  EXPECT_DOUBLE_EQ(light_sensor->get_reading(),999.2)
  << "FAIL : Light Get/Set not properly working for reading";
  light_sensor->set_reading(1000.0);
  EXPECT_DOUBLE_EQ(light_sensor->get_reading(),1000.0)
  << "FAIL : Light Get/Set not properly working for max reading";
}


TEST_F(LightSensorTests, ReadingCalculation){
  light_sensor->set_reading(0.0);
  light_sensor->set_position(300.0,300.0);
  double test_value = TestCalculateReading({304.0,304.0});
  light_sensor->CalculateReading({304.0,304.0});
  EXPECT_DOUBLE_EQ(light_sensor->get_reading(),test_value)
  << "FAIL : Reading Calculation has failed for one lamp";


  light_sensor->set_reading(0.0);
  test_value = TestCalculateReading({400.0,400.0}) +
  TestCalculateReading({563.5,663.6}) + TestCalculateReading({243.0,423.04})
  + TestCalculateReading({538.5,736.64});
  light_sensor->CalculateReading({400.0,400.0});
  light_sensor->CalculateReading({563.5,663.6});
  light_sensor->CalculateReading({243.0,423.04});
  light_sensor->CalculateReading({538.5,736.64});
  EXPECT_DOUBLE_EQ(light_sensor->get_reading(),test_value)
  << "FAIL : Reading Calculation has failed for four lamps";

  light_sensor->set_reading(0.0);
  light_sensor->set_position(1.1,2.2);
  test_value = TestCalculateReading({500.0,300.0});
  light_sensor->CalculateReading({500.0,300.0});
  //light_sensor->CalculateReading({400.0,350.0});
  EXPECT_DOUBLE_EQ(light_sensor->get_reading(),test_value)
  << "FAIL : Reading Calculation has failed for one lamp";

   // Saturate Sensor
   light_sensor->set_reading(0.0);
   light_sensor->CalculateReading({1.1,2.2});
   EXPECT_DOUBLE_EQ(light_sensor->get_reading(),1000)
  << "FAIL : Saturation test failed";

  light_sensor->set_reading(0.0);
  light_sensor->set_position(30.0,30.0);
  test_value = TestCalculateReading({304.0,304.0});
  light_sensor->CalculateReading({304.0,304.0});
  EXPECT_DOUBLE_EQ(light_sensor->get_reading(),test_value)
  << "FAIL : Reading Calculation has failed for one lamp";

}

#endif
