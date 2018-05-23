/**
 * @file light_sensor.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 *
 */
#ifndef SRC_LIGHT_SENSOR_H_
#define SRC_LIGHT_SENSOR_H_

/*Includes
*/
#include "src/pose.h"
#include "src/sensor.h"


NAMESPACE_BEGIN(csci3081);
/**
* @brief Class extends from Sensor and provides CalculateReading
*/ 
class LightSensor : public Sensor {
 public:
      /**
      *@brief LightSensor constructor
      */

      explicit LightSensor(Pose pose) : Sensor(pose) {}

      virtual ~LightSensor() = default;
      // @brief Calculate Reading function using entity position
      void CalculateReading(Pose ent) override;
};


NAMESPACE_END(csci3081);

#endif  // SRC_LIGHT_SENSOR_H_
