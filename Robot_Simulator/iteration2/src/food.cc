/**
 * @file food.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 *
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/food.h"
#include "src/params.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Constructors/Destructor
 ******************************************************************************/
Food::Food() : ArenaImmobileEntity(), captured_(false) {
  set_type(kFood);
  set_color(FOOD_COLOR);
  set_pose(FOOD_INIT_POS);
  set_radius(FOOD_RADIUS);
}

/*******************************************************************************
 * Member Functions
 ******************************************************************************/
void Food::Reset() {
  set_pose(SetPoseRandomly());
  set_color(FOOD_COLOR);
  set_radius(FOOD_RADIUS);
  set_captured(false);
} /* Reset */

NAMESPACE_END(csci3081);
