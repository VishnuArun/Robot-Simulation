/**
 * @file food_sensor.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 *
 */
#include <cmath>
#include "src/params.h"
#include "src/food_sensor.h"


NAMESPACE_BEGIN(csci3081);

/** Euclidean Distance formula sqrt( (x1-x0)^2 + (y1-y0)^2 )
*  if food too close sensor report maximum value
*  Reading range [0,1000]
*/
void FoodSensor::CalculateReading(Pose ent) {
  double reading = get_reading();
  reading = (1200.0 / pow(1.08, (pow((pow((get_position().x - ent.x), 2.0) +
              pow((get_position().y - ent.y), 2.0)), 0.5) - FOOD_RADIUS)));
  if (reading > 1000.0) {
    set_reading(1000.0);
  } else {
    set_reading(reading);
  }
}

NAMESPACE_END(scsi3081);
