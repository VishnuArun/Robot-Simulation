/**
 * @file sensor.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 *
 */
#ifndef SRC_SENSOR_H_
#define SRC_SENSOR_H_


#include "src/pose.h"




NAMESPACE_BEGIN(csci3081);

/**
* @brief Provides Framework for robot sensors. Includes Position and reading
attributes.
*/

class Sensor {
 public:
    /**
     * @brief Constructor.
     */
    explicit Sensor(Pose p) : position_(p), reading_(0.0) {}
    /**
   * @brief Default destructor -- as defined by compiler.
   */
    virtual ~Sensor() = default;

    // @brief Set sensor position
    void set_position(double inx, double iny) {
      position_.x = inx;
      position_.y = iny;
    }

    void set_pose(Pose p) {
      position_.x = p.x;
      position_.y = p.y;
    }

    Pose get_position() {return position_;}

    // @brief Reading function -- returns reading between 0-1000
    double get_reading() {return reading_;}
    /**
    * @brief Helper Function -- Will be used in conjuction
    * with calculate_reading depending on speicific sensor
    */
    void set_reading(double reading) {reading_ = reading;}

    void Reset() {reading_ = 0.0;}

    /**
    * @brief Calculate Sensor Reading -- logic for
    * reading calculation will be placed here
    */
    virtual void CalculateReading(Pose ent) = 0;

 private:
    // @brief position of sensor
    Pose position_;
    double reading_;
};


















NAMESPACE_END(csci3081);

#endif  // SRC_SENSOR_H_
