/**
 * @file obstacle.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

#ifndef SRC_OBSTACLE_H_
#define SRC_OBSTACLE_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <string>

#include "src/arena_mobile_entity.h"
#include "src/common.h"
#include "src/entity_type.h"
#include "src/pose.h"
#include "src/motion_handler.h"
#include "src/motion_behavior_differential.h"


/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Class Definitions
 ******************************************************************************/
/**
 * @brief Class representing an immobile obstacle within the Arena.
 *
 * Since obstacles are immobile, the Obstacle class is very simple.
 */
class Obstacle : public ArenaMobileEntity {
 public:
  /**
   * @brief Constructor.
   */
  Obstacle();

  /**
   * @brief Get the name of the Obstacle for visualization purposes, and to
   * aid in debugging.
   */

  std::string get_name() const override {
    return "Obstacle" + std::to_string(get_id());
  }
  /**
  * @brief override reset custom to obstacle
  */
  void Reset() override;
  /**
  * @brief Update timestep fo grpahics purposes
  *
  * @param dt number of time steps since last update
  *
  *
  */
  void TimestepUpdate(unsigned int dt) override;
  /**
  * @brief Handle collisions with objects
  */
  void HandleCollision(EntityType object_type, ArenaEntity * object = NULL);


 private:
  // @brief Folowing variable used since Obstacle is a mobile entity
  MotionHandler motion_handler_;

  MotionBehaviorDifferential motion_behavior_;

  /**
   @ brief Following Variables used for ArcReverse fucntion where
   Obstacle reverse in an arc upon contact
  */
  bool ArcReverse_;

  // Variable acts as a counter for how long arc behavior lasts
  int ArcTime_;
};

NAMESPACE_END(csci3081);

#endif  // SRC_OBSTACLE_H_
