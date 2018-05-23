/**
 * @file food_sensor.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 *
 */
#ifndef SRC_FOOD_SENSOR_H_
#define SRC_FOOD_SENSOR_H_

/*Includes
*/
#include "src/pose.h"
#include "src/sensor.h"


NAMESPACE_BEGIN(csci3081);

  /**
  * @brief Food sensor class extends Sensor class. implements CalculateReading
  Specific to this sensor.
  */
  class FoodSensor : public Sensor {
   public:
      /**
      *@brief FoodSensor constructor
      */

      explicit FoodSensor(Pose pose) : Sensor(pose) {}

      virtual ~FoodSensor() = default;
      // @brief Calculate Reading function using entity position
      void CalculateReading(Pose ent) override;
  };
NAMESPACE_END(csci3081);

#endif  // SRC_FOOD_SENSOR_H_
